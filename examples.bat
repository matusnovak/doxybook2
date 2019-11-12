"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/mkdocs-readthedocs/docs ^
    --config example/mkdocs-readthedocs/.doxybook2/config.json
    
"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/mkdocs-material/docs ^
    --config example/mkdocs-material/.doxybook2/config.json

"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/mkdocs-bootstrap/docs ^
    --config example/mkdocs-bootstrap/.doxybook2/config.json

"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/hugo-learn/content ^
    --config example/hugo-learn/.doxybook2/config.json ^
    --templates example/hugo-learn/.doxybook2/templates

"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/hugo-book/content ^
    --config example/hugo-book/.doxybook2/config.json ^
    --templates example/hugo-book/.doxybook2/templates

"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/vuepress ^
    --config example/vuepress/.doxybook2/config.json

"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/gitbook ^
    --config example/gitbook/.doxybook2/config.json

"build\src\Doxybook2Cli\Debug\doxybook2.exe" ^
    --input example/doxygen/xml ^
    --output example/json ^
    --json \
    --config-data "{""linkSuffix:"" """"}"
