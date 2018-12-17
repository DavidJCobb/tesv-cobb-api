import * as dom from "./module-dom-utilities.js";
import {PapyrusClass}  from "./module-papyrus-class.js";
import {PapyrusMethod} from "./module-papyrus-method.js";
import {PromiseShim}   from "./module-pending-promise.js";

export let data = {
   classes:  [],
   concepts: [],
};

let index_document = null;
export {index_document as document};

let class_promise = new PromiseShim();
let __c = class_promise.readonly(); // yOu CaN oNlY hAvE eXpOrTs At ThE tOp LeVeL sCoPe
export {__c as class_promise};

let index_promise = dom.fetchXML("index.xml").then(function (/*XMLDocument*/ doc) {
   let node = doc.querySelector("root");
   if (!node || doc.querySelectorAll("parsererror").length)
      throw "Unable to find requested node.";
   index_document = doc;
   //
   let child;
   if (child = node.querySelector("title"))
      data.title = child.textContent;
   if (child = node.querySelector("description"))
      data.description = child.innerHTML;
   //
   data.abstractTypes = {};
   for(let type of node.querySelectorAll("abstract-types>type")) {
      let name = type.getAttribute("name");
      if (name)
         data.abstractTypes[name] = type.innerHTML;
   }
   let _promises = [];
   for(let item of node.querySelectorAll("classes>class")) {
      let name = item.getAttribute("name");
      if (!name)
         continue;
      let entry = {};
      entry.name = name;
      entry.file = item.getAttribute("file");
      let i = data.classes.push(entry) - 1;
      data.classes[name] = entry;
      _promises.push(
         dom.fetchXML("classes/" + entry.name + ".xml").then(function(/*XMLDocument*/ doc) {
            let d;
            if (doc.documentElement.nodeName.toLowerCase() == "class") {
               let file = data.classes[i].file;
               d = data.classes[i] = new PapyrusClass(doc.documentElement);
               d.file = file;
            }
            return d;
         }).catch(function(e) { return e})
      );
   }
   Promise.all(_promises).then(function(values) {
      class_promise.resolve(values);
   },
   function(fail) {
      class_promise.reject(fail);
   });
   for(let item of node.querySelectorAll("concepts>concept")) {
      let name = item.getAttribute("name");
      if (!name)
         continue;
      let entry = {};
      entry.name = name;
      entry.file = item.getAttribute("file");
      data.concepts.push(entry);
      data.concepts[name] = entry;
   }
   return doc;
});
export {index_promise as promise};

export /*PapyrusMethod*/ function getClass(className) {
   return class_promise.then(function() {
      for(let classObj of data.classes)
         if (classObj.name == className)
            return classObj;
      return null;
   });
}
export /*PapyrusMethod*/ function getClassOrMethod(className, methodName) {
   return class_promise.then(function() {
      for(let classObj of data.classes)
         if (classObj.name == className) {
            if (!methodName)
               return classObj;
            for(let method of classObj.methods)
               if (method.name == methodName)
                  return method;
            break;
         }
      return null;
   });
}