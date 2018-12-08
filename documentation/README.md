# Documentation

I've found that the easiest way to write all of the documentation quickly is to write it in XML, and use JavaScript to convert that to a readable HTML page. Unfortunately, Google Chrome is the only browser that doesn't allow web requests from local files, but it's also the most popular, so we can't actually distribute the documentation in this state. We'll have to use the XML/JS approach to "bake" finished, non-scripted files, which we can then distribute.

One small luxury is that our XML/JS doesn't have to be perfect. It's fine if we use page reloads instead of just regenerating the whole DOM from scratch. It's fine if the code isn't lightning-fast, or if it's a little redundant, as long as it's maintainable and it helps us get the *real* job -- baking the non-scripted files -- done fast and well.
