# AdamLight

![screenshot](screenshot2.png?raw=true "engine screenshot showing shadows, normal mapping, screen-space reflections")

This is a straightforward deferred graphics engine in C++ / OpenGL. I wrote it **a long time ago** (I was 15 years old) — needless to say, if I were to write another, I would do things very differently.

It has some basic features:

* Deferred rendering
* Shadow mapping
* Normal mapping
* Gamma-correct rendering (I think?)
* Screen-space reflections
* .obj/.mtl model loading

I have a few other versions floating around where I was playing with transmission, GI, tone mapping, etc., but this was the most coherent VS project in my backups folder.
The only libraries I used were GLEW, GLFW, and GLM. The code is terribly un-forkable, but some parts of it (e.g. `GLUtil.cpp`) might be useful for those learning OpenGL for the first time.
