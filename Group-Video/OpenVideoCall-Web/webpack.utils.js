const path = require('path');
const url = require('url');
const fs = require('fs');
const ExtractTextPlugin = require('extract-text-webpack-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');

exports.cssLoaders = function() {
  let prod = process.env.NODE_ENV === 'production';
  const styleLoader = {
    loader: 'style-loader'
  };
  const cssLoader = {
    loader: 'css-loader',
    options: {
      sourceMap: prod
    }
  };
  const postcssLoader = {
    loader: 'postcss-loader',
    options: {
      sourceMap: prod
    }
  };
  // Generate loader string to be used with extract text plugin
  function generateLoaders(loader, loaderOptions) {
    const loaders = [cssLoader, postcssLoader];

    if (loader) {
      loaders.push({
        loader: loader + '-loader',
        options: Object.assign({}, loaderOptions, {
          sourceMap: prod
        })
      });
    }

    // Extract CSS when that option is specified
    // (which is the case during production build)
    if (prod) {
      return ExtractTextPlugin.extract({
        use: loaders,
        publicPath: '../../',
        fallback: 'style-loader'
      });
    }
    return [styleLoader, ...loaders];
  }

  return {
    css: generateLoaders(),
    postcss: generateLoaders(),
    sass: generateLoaders('sass', {
      indentedSyntax: true
    }),
    scss: generateLoaders('sass')
  };
};

// Generate loaders
exports.styleLoaders = function() {
  const output = [];
  const loaders = exports.cssLoaders();

  for (const extension in loaders) {
    if (extension) {
      const loader = loaders[extension];
      output.push({
        test: new RegExp('\\.' + extension + '$'),
        use: loader
      });
    }
  }
  return output;
};

exports.getPublicPath = function() {
  let urlString = require('./package.json').homepage;
  let pathname = url.parse(urlString).pathname;
  if (!pathname) {
    return '/';
  }

  return /\/$/.test(pathname) ? pathname : pathname + '/';
};

exports.getEntry = function() {
  const ROUTES_PATH = path.resolve(__dirname, './src/pages');
  const routesArray = fs.readdirSync(ROUTES_PATH);
  let result = {};
  for (let item of routesArray) {
    result[item] = `@/pages/${item}/${item}.js`;
  }
  return result;
};

exports.getHtml = function() {
  return Object.keys(exports.getEntry()).map(
    key =>
      new HtmlWebpackPlugin({
        filename: `${key}.html`,
        template: `src/pages/${key}/${key}.html`,
        inject: true,
        chunks: ['manifest', 'vendor', key]
      })
  );
};
