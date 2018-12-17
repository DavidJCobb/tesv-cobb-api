import * as dom from "./module-dom-utilities.js";

//
// DOCUMENTATION
//
// TAG: T-ATTR
//  - Overwrites an attribute (specified by its own "f" attr-
//    ibute) on its parent element. Its text content (after 
//    applying all other template-system tags) becomes the 
//    attribute value.
//
// TAG: T-F
//  - Retrieve the field specified by the "f" attribute. You 
//    can refer to subfields in an object using ":" as a sep-
//    arator, e.g. "arg:name".
//
// TAG: T-IF
//  - Contents are deleted if the condition specified in the 
//    "f" attribute is not met. The condition can be a field 
//    in the object (boolean conversion for most types; for 
//    arrays, true if they're not empty), or a flag.
//
//    Flags are prefixed with "::", possibly followed by a 
//    "!" to invert them (e.g. "::!flag"). Available flags:
//
//       last-in-set: The last item rendered using a t-set.
//
// TAG: T-SET
//  - The "f" attribute specifies a list to loop over; the 
//    "as" attribute specifies an identifier for list items. 
//    The contents of the T-SET element will be duplicated 
//    for each item in the list, with T- data processed 
//    independently for each.
//
// ATTRIBUTE: T-IF
//  - An attribute that can be placed on any element. Behaves 
//    like the "f" attribute on "t-if" elements.
//

export function domToObj(node) {
   if (!node.attributes.length && !node.children.length)
      return node.textContent.trim();
   let o = {};
   for(let i=0;i<node.attributes.length;i++) {
      let a = node.attributes[i];
      o[a.name] = a.value;
   }
   for(let child of node.children) {
      let e = o[child.nodeName];
      if (e) {
         if (!(e instanceof Array))
            o[child.nodeName] = [e];
         o[child.nodeName].push(domToObj(child));
      } else
         o[child.nodeName] = domToObj(child);
   }
   let keys = Object.keys(o);
   if (keys.length == 1 && o[keys[0]] instanceof Array)
      o = o[keys[0]];
   return o;
}

class _Scope {
   constructor() {
      this.vars = [];
   }
   /*_Scope*/ copy() {
      let copy = new _Scope();
      for(let v of this.vars) {
         copy.push(v.name, v.data);
         v.flags.forEach(flag => copy.modify_flag(flag, true));
      }
      return copy;
   }
   /*void*/ push(/*string*/ name, obj) {
      this.vars.push({name: name, data: obj, flags: new Set()});
   }
   /*void*/ pop() {
      this.vars.pop();
   }
   /*bool*/ has_flag(/*variant*/ flag) {
      if (!this.vars.length)
         return false;
      this.vars[this.vars.length - 1].flags.has(flag);
   }
   /*void*/ modify_flag(/*variant*/ flag, /*bool*/ state) {
      if (!this.vars.length)
         return;
      if (state)
         this.vars[this.vars.length - 1].flags.add(flag);
      else
         this.vars[this.vars.length - 1].flags.delete(flag);
   }
   /*Object*/ lookup(/*string*/ term) {
      for(let i = this.vars.length - 1; i >= 0; i--) {
         let v = this.vars[i];
         let names = term.split("||");
         for(let n of names)
            if (v.name == n)
               return v.data;
      }
      return void 0;
   }
   /*variant*/ resolve(/*string*/ path) {
      path = path.split(":");
      let data = this.lookup(path[0]);
      let i = 1;
      if (data === void 0) {
         data = this.lookup("");
         if (data === void 0)
            return void 0;
         i = 0;
      }
      for(; i < path.length; i++) {
         {
            let d;
            let names = path[i].split("||");
            for(let n of names) {
               d = data[n];
               if (d !== void 0 && (names.length == 1 || d !== ""))
                  break;
            }
            data = d;
         }
         if (!data)
            return void 0;
      }
      return data;
   }
   /*Object*/ get_frame_by_flag(/*variant*/ flag) {
      if (!this.vars.length)
         return void 0;
      for(let i = this.vars.length - 1; i >= 0; i--) {
         let v = this.vars[i];
         if (v.flags.has(flag))
            return { name: v.name, data: v.data, flags: new Set(v.flags) };
      }
      return void 0;
   }
   /*Object*/ get_last_frame(/*variant*/ flag) {
      if (!this.vars.length)
         return void 0;
      let v = this.vars[this.vars.length - 1];
      return { name: v.name, data: v.data, flags: new Set(v.flags) };
   }
}

let FLAG_IS_SET_ELEMENT = "set-item";
let FLAG_LAST_IN_SET    = "last-in-set";
class TemplateWalker {
   constructor(node, obj, _outerWalker) {
      this._root = node;
      this._nodes = Array.from(node.querySelectorAll("t-attr, t-f, t-if, t-set, [t-if]"));
      this._current = this._nodes.shift();
      this.scope  = new _Scope();
      this.scope.push("", obj);
      if (_outerWalker)
         this.scope = _outerWalker.scope.copy();
   }
   /*Node*/ get current() {
      return this._current;
   }
   /*Node*/ next() {
      this._current = this._nodes.shift();
      return this._current;
   }
   /*variant*/ lookup(/*string*/ field) {
      return this.scope.resolve(field);
   }
   /*string*/ get type() {
      let node = this.current;
      if (!node)
         return null;
      if (node.nodeName.substring(0, 2).toLowerCase() == "t-")
         return node.nodeName.toLowerCase();
      if (node.hasAttribute("t-if"))
         return "t-if";
   }
   /*bool*/ check_flag(flag) {
      let stackframe = this.scope.get_last_frame();
      switch (flag) {
         case FLAG_LAST_IN_SET:
            stackframe = this.scope.get_frame_by_flag(FLAG_IS_SET_ELEMENT);
      }
      if (!stackframe)
         return false;
      return stackframe.flags.has(flag);
   }
}

export function execute(node, obj, _outerWalker) {
   let walker = new TemplateWalker(node, obj, _outerWalker);
   for (let node = walker.current; node; node = walker.next()) {
      //if (dom.isRemoved(node))
      //   continue;
      switch (walker.type) {
         case "t-attr": {
            let target = node.parentNode;
            let attr   = node.getAttribute("f");
            target.removeChild(node);
            execute(node, obj, walker);
            target.setAttribute(attr, node.textContent);
         } break;
         case "t-f": {
            let data = walker.lookup(node.getAttribute("f"));
            let wrap = node.getAttribute("wrap-if-bare");
            let type = node.getAttribute("type") || "text";
            switch (type) {
               case "text": case null: {
                  if (wrap) {
                     let p = document.createElement(wrap);
                     p.appendChild(document.createTextNode(data));
                     node.replaceWith(p);
                  } else
                     node.replaceWith(document.createTextNode(data));
               } break;
               case "html": {
                  let root = dom.parseAndSanitize(data, null, ["t-f"]);
                  if (root.querySelector("t-f")) {
                     execute(root, obj, walker);
                  }
                  if (data && wrap && dom.hasBareText(root)) { // text content as a bare text node
                     let p = root.ownerDocument.createElement(wrap);
                     while (root.firstChild)
                        p.appendChild(root.firstChild);
                     root.appendChild(p);
                  }
                  node.replaceWith(...root.childNodes);
               } break;
            }
         } break;
         case "t-if": {
            let condition = node.getAttribute((node.nodeName.toLowerCase() == "t-if") ? "f" : "t-if");
            let truth;
            if (condition.substring(0, 2) == "::") {
               let invert = condition.charAt(2) == "!";
               let flag   = condition.substring(2 + (invert ? 1 : 0));
               truth = walker.check_flag(flag);
               if (invert)
                  truth = !truth;
            } else {
               let data = walker.lookup(condition);
               truth = data && !(data instanceof Array && !data.length);
            }
            if (!truth)
               node.parentNode.removeChild(node)
            else
               dom.unwrap(node);
         } break;
         case "t-set": {
            let field = node.getAttribute("f");
            let as    = node.getAttribute("as") || field;
            let list  = walker.lookup(field);
            if (list === void 0)
               list = [];
            else
               list = Array.from(list);
            if (list.length) {
               let frag = document.createDocumentFragment();
               for (let i = 0; i < list.length; i++) {
                  let item = list[i];
                  walker.scope.push(as, item);
                  walker.scope.modify_flag(FLAG_IS_SET_ELEMENT, true);
                  if (i == list.length - 1)
                     walker.scope.modify_flag(FLAG_LAST_IN_SET, true);
                  //
                  let current = document.createDocumentFragment();
                  for(let i=0;i<node.childNodes.length;i++)
                     current.appendChild(node.childNodes[i].cloneNode(true));
                  execute(current, item, walker);
                  frag.appendChild(current);
                  walker.scope.pop();
               }
               node.replaceWith(frag);
            } else {
               node.parentNode.removeChild(node);
            }
         } break;
      }
   }
   
}