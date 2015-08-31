Debian source package for Drumgizmo
-----------------------------------

####To build:
`mkdir build`

`cd build`

`git clone https://github.com/joulez/drumgizmo`

`cd drumgizmo`

Ensure you have the `build-essential` package installed. Then:

`dpkg-buildpackage -b -uc`

After installing the relevent development packages - you should then have a `deb` file within the `build` directory you just created and is ready to install via `dpkg -i {file}`.
