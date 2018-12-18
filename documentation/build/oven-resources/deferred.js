//
// Makes it possible to register for a promise that has not yet been 
// made.
//
export /*Promise*/ function Deferred() {
   let resolve;
   let reject;
   let promise = new Promise(function(res, rej) {
      resolve = res;
      reject  = rej;
   });
   promise.resolve = resolve;
   promise.reject  = reject;
   promise.promise =
      function() {
         return Promise.race([this]);
      };
   return promise;
}