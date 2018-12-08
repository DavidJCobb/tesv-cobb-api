//
// Make it possible to register for a promise that has not yet been 
// made.
//
export /*Promise*/ function PromiseShim() {
   let resolve;
   let reject;
   let promise = new Promise(function(res, rej) {
      resolve = res;
      reject  = rej;
   });
   promise.resolve = resolve;
   promise.reject  = reject;
   promise.readonly =
      function() {
         return Promise.race([this]);
      };
   return promise;
}