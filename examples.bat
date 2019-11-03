"build\src\DoxydownCli\Debug\doxydown.exe" ^
    --input example/doxygen/xml ^
    --output example/mkdocs-readthedocs/docs ^
    --config example/mkdocs-readthedocs/.doxydown/config.json
    
"build\src\DoxydownCli\Debug\doxydown.exe" ^
    --input example/doxygen/xml ^
    --output example/mkdocs-material/docs ^
    --config example/mkdocs-material/.doxydown/config.json

"build\src\DoxydownCli\Debug\doxydown.exe" ^
    --input example/doxygen/xml ^
    --output example/mkdocs-bootstrap/docs ^
    --config example/mkdocs-bootstrap/.doxydown/config.json

"build\src\DoxydownCli\Debug\doxydown.exe" ^
    --input example/doxygen/xml ^
    --output example/hugo-learn/content ^
    --config example/hugo-learn/.doxydown/config.json ^
    --templates example/hugo-learn/.doxydown/templates

"build\src\DoxydownCli\Debug\doxydown.exe" ^
    --input example/doxygen/xml ^
    --output example/hugo-book/content ^
    --config example/hugo-book/.doxydown/config.json ^
    --templates example/hugo-book/.doxydown/templates

"build\src\DoxydownCli\Debug\doxydown.exe" ^
    --input example/doxygen/xml ^
    --output example/vuepress ^
    --config example/vuepress/.doxydown/config.json

