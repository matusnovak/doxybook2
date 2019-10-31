var ghpages = require('gh-pages');
ghpages.publish(process.env.DIST, function(err) {});
