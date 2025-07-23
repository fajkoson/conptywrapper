set(WHDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#configure the file version
configure_file(setup.py.in ${WHDIR}/setup.py @ONLY)

configure_file(buildwheel.bat ${WHDIR}/buildwheel.bat COPYONLY)
configure_file(README.md ${WHDIR}/README.md COPYONLY)
configure_file(MANIFEST.in ${WHDIR}/MANIFEST.in COPYONLY)
configure_file(__init__.py ${WHDIR}/pseudoconsole/__init__.py COPYONLY)