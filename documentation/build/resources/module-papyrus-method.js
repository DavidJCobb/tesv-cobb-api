// module

export class PapyrusMethod {
   constructor(a) {
      this.__construct_default();
      if (a instanceof Element && a.nodeName == "method") {
         this.name        = a.getAttribute("name");
         this.returnType  = a.getAttribute("returns") || "";
         let node;
         {
            node = a.parentNode;
            while (node && node.nodeName != "class")
               node = node.parentNode;
            if (node)
               this.className = node.getAttribute("name");
         }
         if (node = a.querySelector("method>blurb"))
            this.blurb = node.innerHTML;
         if (node = a.querySelector("method>description"))
            this.description = node.innerHTML;
         if (node = a.querySelector("method>args"))
            for (let i of node.children) {
               let entry = {};
               entry.type = i.getAttribute("type");
               entry.abstractType = i.getAttribute("abstract-type");
               entry.name = i.getAttribute("name");
               entry.defaultValue = i.getAttribute("default");
               entry.description = i.innerHTML || "";
               this.args.push(entry);
            }
         if (node = a.querySelector("method>example-data"))
            this.setExample(node);
         else
            this.setDefaultExample();
         if (node = a.querySelectorAll("method>related"))
            for (let i of node) {
               let entry = {};
               switch (i.getAttribute("type")) {
                  case "method":
                     entry.className = i.getAttribute("class") || this.className;
                     entry.name      = i.getAttribute("name");
                     entry.classFile = i.getAttribute("class-file") || this.classFile || "";
               }
               this.related.push(entry);
            }
      }
   }
   __construct_default() {
      this.className = "";
      this.name = "";
      this.returnType = "";
      this.args = [];
      this.blurb = "";
      this.description = "";
      this.example = "";
      this.related = [];
   }
   //
   /*void*/ setFile(/*string*/ file) {
      this.classFile = file;
      for(let rel of this.related) {
         if (!rel.classFile)
            rel.classFile = file;
      }
   }
   //
   /*string*/ get syntax() {
      let out = "";
      if (this.returnType)
         out += this.returnType + " ";
      out += "Function " + this.name + "(";
      for(let i=0;i<this.args.length;i++) {
         let a = this.args[i];
         out += a.type + " " + a.name;
         if (a.defaultValue)
            out += " = " + a.defaultValue;
         if (i + 1 < this.args.length)
            out += ", ";
      }
      out += ") Global Native";
      return out;
   }
   /*void*/ setDefaultExample() {
      let out = "";
      if (this.returnType)
         out += this.returnType + " result = ";
      out += this.className + "." + this.name + "(";
      for(let j = 0; j < this.args.length; j++) {
         let a = this.args[j];
         out += (a.defaultValue || a.name);
         if (j + 1 < this.args.length)
            out += ", ";
      }
      out += ")";
      this.example = out;
   }
   /*void*/ setExample(/*Element<example-data>*/ root) {
      let out = "";
      for(let i of root.children) {
         switch (i.nodeName.toLowerCase()) {
            case "call": {
               if (this.returnType) {
                  out += this.returnType + " ";
                  out += i.getAttribute("return-var") || "result";
                  out += " = ";
               }
               out += this.className + "." + this.name + "(";
               //
               let sep = "";
               if (i.getAttribute("line-breaks") == "yes") {
                  sep = "\n   ";
                  out += sep;
               }
               //
               if (!i.children.length) {
                  for(let j = 0; j < this.args.length; j++) {
                     let a = this.args[j];
                     let isString = (a.type.toLowerCase() == "string");
                     if (isString)
                        out += '"';
                     out += (a.defaultValue || a.name);
                     if (isString)
                        out += '"';
                     if (j + 1 < this.args.length)
                        out += ", " + sep;
                  }
                  out += ")";
               } else {
                  let list  = i.querySelectorAll("call>arg");
                  let first = true;
                  let exclusions = -1;
                  for(let j = 0; j < this.args.length; j++) {
                     let a    = this.args[j];
                     let node = list[j];
                     if (node && node.getAttribute("exclude") == "true") {
                        exclusions = j;
                        continue;
                     }
                     if (!first)
                        out += ", " + sep;
                     else
                        first = false;
                     if (exclusions >= 0 || (node && node.getAttribute("force-name") == "true")) {
                        out += a.name + " = ";
                     }
                     let isString = (a.type.toLowerCase() == "string");
                     if (isString)
                        out += '"';
                     if (node) {
                        let v = node.getAttribute("value");
                        if (isString)
                           v = v.replace(/\\/g, "\\\\")
                        out += v;
                     } else {
                        out += (a.defaultValue || a.name);
                     }
                     if (isString)
                        out += '"';
                     /*if (j + 1 < this.args.length)
                        out += ", " + sep;*/
                  }
                  if (i.getAttribute("line-breaks") == "yes")
                     out += "\n";
                  out += ")";
               }
            } break;
            case "comment": {
               out += ";\n";
               let t = i.innerHTML.trim().split(/\r?\n/);
               for(let l of t)
                  out += "; " + l.trim() + "\n";
               out += ";";
            } break;
            case "raw": {
               out += i.textContent.trim();
            } break;
         }
         out += "\n";
      }
      this.example = out;
   }
   /*void*/ updateAbstractTypes(map) {
      for(let arg of this.args) {
         let type = arg.abstractType;
         if (!type)
            continue;
         if (arg.description)
            continue;
         if (map[type])
            arg.description = map[type];
      }
   }
}