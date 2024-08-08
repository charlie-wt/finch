* [x] x, y, z aliases for vec storage access
* [x] do origins more properly
* [~] line clipping?
* [x] filled-shading
    * [x] triangle primitive
    * [x] wavefront-style representation
    * [x] z-buffer
    * [x] shading (prob via dithering)
    * [x] back-face culling
* [x] reconcile `BrailleCanvas` & `CharCanvas`
    * [x] move the `render_fn`s out of `Shader`
    * [x] don't have `Shader`s own their `Framebuffer`s
    * [x] deal with canvas resolutions better
* [ ] reconcile `TextCanvas` & `BrailleCanvas`
    * not sure if worth trying to do this
* [x] frame timer (w/ av. reporting)
* [ ] colour
    * have set(xy, col) & unset(xy)
    * store colour per block
        * prob easiest to to add to Block? either way, if
          we want colour to be optional, it'll have to be
          a `vector<optional<rgb>>` or similar...
    * when calling set(xy, col), update the block's colour
      (could do average based on number of set dots in the
      block, or splatting on top)
    * when drawing the block, blocks with a colour are
      drawn with that colour
    * this means calling only set(xy, on) will set dots on
      the block, but no colour for it; when drawing, it'll
      fall back to drawing with the foreground colour
    * ...sounds like ncurses doesn't support truecolour,
      so may have to get rid of it...
* [ ] something non-mesh-based
    * (mostly just to try out, and make any necessary
      conveniences)
    * [ ] simple fluid sim?
    * [ ] sprite rendering
* [ ] movable camera
* [ ] particles?
* [x] proper makefile
* [x] put on github/lab

