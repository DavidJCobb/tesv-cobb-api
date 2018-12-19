# Documentation

I've found that the easiest way to write all of the documentation quickly is to write it in XML, and use JavaScript to convert that to a readable HTML page. Unfortunately, Google Chrome is the only browser that doesn't allow web requests from local files, but it's also the most popular, so we can't actually distribute the documentation in this state. We'll have to use the XML/JS approach to "bake" finished, non-scripted files, which we can then distribute.

One small luxury is that our XML/JS doesn't have to be perfect. It's fine if we use real hyperlinks instead of just regenerating the whole DOM from scratch. It's fine if the code isn't lightning-fast, or if it's a little redundant, as long as it's maintainable and it helps us get the job done. The only thing that matters is the relative quality of the final product -- the "baked" static pages.

## Build process

I'll warn you right now that this process is ugly. I've never done something like this before; I ran into some unexpected obstacles and worked around them with a focus on speed rather than perfection. If the "template and bake pages" system were *itself* the project I'm working on -- something I was contributing to the public to use or learn from -- I'd aim for perfection, but as it is, the literal goal was to make something that was better than nothing.

Anyway, open the "oven" HTML file (not in Google Chrome) and then click the "Zip!" button. This will use an IFRAME to request each of the documentation pages, "bake" them, and offer you a ZIP file containing all of these pages to save. Note that this archive doesn't include the "resources" folder or the CSS.

You can also browse and bake single pages, by navigating within the IFRAME and using the "Bake!" and "Revert!" buttons. The "Bake!" button copies the HTML of the baked page into the textbox on the bottom, for you to copy and save.
