paintbug
========

3D mesh painter

TODO
--------
* refactor duplicate code from mesh-render/bake-render shaders
* keep from painting on one panel if other dirty
* remove floating gl* references, use inheritance instead
* optimization: create paint FBO only when needed
* show correct paint color for overlay brush
* fix "blurring" that happens to paint buffer on brush drags
* redirect key events to correct widget
* prevent painting through the mesh
* clean up texture/mesh ownership
* fix OpenGL context sharing across app
* swap Point2 typedef for QPointF
* add anti-aliasing to viewport
