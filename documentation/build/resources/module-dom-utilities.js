export /*void*/ function clear(/*Node*/ target) {
   if (target.childNodes.length)
      target.removeChild(...target.childNodes);
}
export /*Promise*/ function fetchXML(input, init) {
   return fetch(input, init)
      .then(response => response.text())
      .then(
         function(s) {
            let /*XMLDocument*/ doc = (new window.DOMParser()).parseFromString(s, "text/xml");
            doc.sourceFile = input;
            if (doc.querySelectorAll("parsererror").length) {
               let e = new Error();
               e.parsererror = doc.querySelectorAll("parsererror");
               throw e;
            }
            return doc;
         }
      );
}
export /*bool*/ function hasBareText(/*Node*/ target) {
   for(let node of target.childNodes)
      if (node.nodeName == "#text" && node.textContent.trim())
         return true;
   return false;
}
export /*bool*/ function isRemoved(/*Node*/ target) {
   let p = target;
   while (p.parentNode)
      p = p.parentNode;
   if (p == target)
      return true;
   if (p instanceof Document)
      return false;
   if (p instanceof DocumentFragment)
      return false;
   return true;
}
export /*Element root*/ function parseAndSanitize(/*string*/ html, /*string*/ mime, /*array<lowercase-strings>*/ extraTags) {
   if (!mime)
      mime = "text/html";
   let parser = new DOMParser();
   let doc = parser.parseFromString(html, mime);
   let root = doc.body;
   root.querySelectorAll("*").forEach(function(n) {
      switch (n.nodeName.toLowerCase()) {
         case "a":
         case "b":
         case "blockquote":
         case "code":
         case "col":
         case "colgroup":
         case "dd":
         case "del":
         case "dfn":
         case "dl":
         case "dt":
         case "em":
         case "h1":
         case "h2":
         case "h3":
         case "h4":
         case "h5":
         case "h6":
         case "hgroup":
         case "i":
         case "ins":
         case "li":
         case "ol":
         case "p":
         case "pre":
         case "strong":
         case "table":
         case "tbody":
         case "td":
         case "th":
         case "thead":
         case "tr":
         case "ul":
         case "var":
         case "wbr":
            break;
         default:
            if (extraTags && extraTags.indexOf(n.nodeName.toLowerCase()) >= 0)
               break;
            n.replaceWith(document.createTextNode(n.outerHTML));
      }
   });
   return root;
}
export /*void*/ function replaceWithChildrenOf(/*Node*/ target, /*Node*/ source) {
   if (!source.childNodes.length)
      return;
   let last = document.adoptNode(source.firstChild, true);
   target.replaceWith(last);
   while (source.firstChild) {
      let next = document.adoptNode(source.firstChild, true);
      last.parentNode.insertBefore(next, last.nextSibling);
      last = next;
   }
}
export /*void*/ function unwrap(/*Node*/ target) {
   target.replaceWith(...target.childNodes);
}