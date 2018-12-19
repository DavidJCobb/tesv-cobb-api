import {Deferred} from "./deferred.js";

let frame = document.getElementById("frame");
let state = {
   is_zipping: false,
   is_pending: false,
   zip_process: {
      zip: null,
      crawled: new Set(),
      toCrawl: new Set(),
      failed_pages: [],
   },
   pending_load_timeout: 0,
};
let load_callbacks = [];

export async function zip() {
   if (state.is_zipping)
      return null;
   state.is_zipping = true;
   state.zip_process.zip = new JSZip();
   await _load("index.html");
   await _zip_loaded();
   console.log("ZIPSystem: crawling complete.");
   let blob = await state.zip_process.zip.generateAsync({type: "blob" });
   console.log("ZIPSystem: ZIP file generated.");
   //
   let link  = document.createElement("a");
   let url   = URL.createObjectURL(blob);
   link.href = url;
   link.download = name;
   document.body.appendChild(link);
   link.click();
   setTimeout(
      function() {
         document.body.removeChild(link);
         window.URL.revokeObjectURL(url);  
      },
   0);
   state.zip_process.crawled = new Set();
   state.zip_process.toCrawl = new Set();
   state.is_zipping = false;
   console.log("Failed pages:\n\n" + state.zip_process.failed_pages.join("\n"));
}

/*[stem, path]*/ function bakePath(/*URLSearchParams*/ query) {
   let stem = "";
   let path = "";
   if (query.has("method")) {
      stem = "../../";
      path = "classes/" + query.get("class") + "/" + query.get("method") + ".html";
   } else if (query.has("class")) {
      stem = "../";
      path = "classes/" + query.get("class") + ".html";
   } else if (query.has("concept")) {
      stem = "../";
      path = "concepts/" + query.get("concept") + ".html";
   } else {
      path = "index.html";
   }
   return [stem, path];
}
function bakedPathToQuery(path, bake) {
   let match;
   bake = bake ? "&bake" : "";
   if (match = path.match(/classes\/([^\/]+)\.html$/))
      return `index.html?class=${match[1]}${bake}`;
   if (match = path.match(/classes\/([^\/]+)\/([^\/]+)\.html$/))
      return `index.html?class=${match[1]}&method=${match[2]}${bake}`;
   if (match = path.match(/concepts\/([^\/]+)\.html$/))
      return `index.html?concept=${match[1]}${bake}`;
   return `index.html?${bake}`;
}
function sanitizeBakedPath(path) {
   path = decodeURI(path);
   let match;
   if (match = path.match(/classes\/([^\/]+)\.html$/))
      return `classes/${match[1]}.html`;
   if (match = path.match(/classes\/([^\/]+)\/([^\/]+)\.html$/))
      return `classes/${match[1]}/${match[2]}.html`;
   if (match = path.match(/concepts\/([^\/]+)\.html$/))
      return `concepts/${match[1]}.html`;
   return `index.html`;
}


/*Promise*/ function _load(path) {
   console.log("ZIPSystem: loading: ", path);
   let p = new Promise(function(resolve, reject) {
      load_callbacks.push(resolve);
   });
   frame.src = bakedPathToQuery(path, true);
   return p;
}
async function _zip_loaded() {
   //console.log("ZIPSystem: loaded frame; analyzing...");
   let doc = frame.contentDocument;
   {
      let url   = frame.contentWindow.location.href;
      let query = bakedPathToQuery(url);
      let path;
      let stem;
      [stem, path] = bakePath((new URL(url)).searchParams);
      let html  = "<!doctype html>" + frame.contentDocument.documentElement.outerHTML;
      //console.log("ZIPSystem: loaded frame; storing file in ZIP: ", path);
      state.zip_process.zip.file(path, html);
      state.zip_process.crawled.add(path);
      state.zip_process.toCrawl.delete(path);
   }
   doc.querySelectorAll("a[href]").forEach(function(node) {
      if (node.href.indexOf("http") == 0)
         return;
      let path = sanitizeBakedPath(node.href);
      if (!state.zip_process.crawled.has(path))
         state.zip_process.toCrawl.add(path);
   });
   {
      let iterator = state.zip_process.toCrawl.values();
      let next = iterator.next();
      if (next.done)
         return;
      await _load(next.value);
      await _zip_loaded();
   }
}

let __waits = 0;
function __fireLoadCallback() {
   let callbacks = load_callbacks;
   load_callbacks = [];
   for(let c of callbacks)
      if (c instanceof Function)
         c();
      else
         console.warn("load_callbacks contains a non-function: ", c);
}
window.addEventListener("bakecomplete", function(e) {
   if (state.pending_load_timeout !== null) {
      window.clearTimeout(state.pending_load_timeout);
      state.pending_load_timeout = null;
      __fireLoadCallback();
   }
});
frame.addEventListener("load", function(e) {
   //
   // We need to give the contained page time to process and strip templates.
   //
   __waits = 0;
   state.pending_load_timeout = window.setTimeout(function wait(){
      if (!state.is_zipping)
         return;
      state.is_pending = false;
      try {
         let nodes = frame.contentDocument.documentElement.querySelectorAll("template");
         if (nodes.length)
            throw new Error("");
      } catch (e) {
         //
         // The document is inaccessible, OR there is unprocessed template content 
         // in the document indicating that the frame-side scripts need more time.
         //
         if (++__waits < 50) {
            if (__waits == 10) {
               console.log("Framed page is taking a long time to bake...");
            } else if (__waits == 20) {
               console.log("Framed page is taking an unusual amount of time to bake...");
            }
            state.pending_load_timeout = window.setTimeout(wait, 500);
            return;
         } // else give up and just fire the callback
         console.log("Giving up; saving this page even if it's not fully baked.");
         try {
            let url = frame.contentWindow.location.href;
            state.zip_process.failed_pages.push(sanitizeBakedPath(url));
         } catch (e) {}
      }
      state.pending_load_timeout = null;
      __fireLoadCallback();
   }, 500);
});