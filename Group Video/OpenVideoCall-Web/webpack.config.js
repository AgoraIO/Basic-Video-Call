const path = require('path');
const webpack = require('webpack');
const ExtractTextPlugin = require('extract-text-webpack-plugin');
const OptimizeCSSPlugin = require('optimize-css-assets-webpack-plugin');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const UglifyJsPlugin = require('uglifyjs-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const utils = require('./webpack.utils');

module.exports = {
  entry: utils.getEntry(),
  output: {
    path: path.resolve(__dirname, './dist'),
    publicPath: '/',
    filename: 'assets/js/[name].[hash:7].js'
  },
  module: {
    rules: utils.styleLoaders().concat([
      {
        test: /\.html$/,
        loader: 'html-loader'
      },
      {
        test: /\.(js|jsx|mjs)$/,
        include: path.resolve(__dirname, 'src'),
        loader: require.resolve('babel-loader'),
        options: {
          cacheDirectory: true
        }
      },
      {
        test: [/\.(bmp|gif|jpe?g|png|svg)(\?.*)?$/],
        loader: require.resolve('url-loader'),
        options: {
          limit: 10000,
          name: 'assets/img/[name].[hash:7].[ext]'
        }
      },
      {
        test: /\.(mp4|webm|ogg|mp3|wav|flac|aac)(\?.*)?$/,
        loader: 'url-loader',
        options: {
          limit: 10000,
          name: 'assets/media/[name].[hash:7].[ext]'
        }
      },
      {
        test: /\.(woff2?|eot|ttf|otf)(\?.*)?$/,
        loader: 'url-loader',
        options: {
          limit: 10000,
          name: 'assets/fonts/[name].[hash:7].[ext]'
        }
      }
    ])
  },
  resolve: {
    alias: {
      '@': path.join(__dirname, './src')
    },
    extensions: ['*', '.js', '.json']
  },
  devServer: {
    historyApiFallback: true,
    noInfo: true,
    overlay: true
  },
  performance: {
    hints: false
  },
  devtool: '#eval-source-map',
  plugins: [
    // New webpack.ProvidePlugin({
    //   $: 'jquery',
    //   jQuery: 'jquery',
    //   'window.jQuery': 'jquery'
    // }),
    new webpack.NamedModulesPlugin(),
    new webpack.HotModuleReplacementPlugin(),
    new CopyWebpackPlugin([
      {
        from: path.resolve(__dirname, './static'),
        to: 'assets/',
        ignore: ['.*']
      }
    ])
  ].concat(utils.getHtml())
};

if (process.env.NODE_ENV === 'production') {
  module.exports.output.publicPath = './';
  module.exports.devtool = '#source-map';
  module.exports.plugins = (module.exports.plugins || []).concat([
    new webpack.DefinePlugin({
      'process.env': {
        NODE_ENV: '"production"'
      }
    }),
    new CleanWebpackPlugin(['dist']),
    new UglifyJsPlugin({
      sourceMap: true,
      uglifyOptions: {
        warnings: false
      }
    }),
    new webpack.LoaderOptionsPlugin({
      minimize: true
    }),
    // Extract css into its own file
    new ExtractTextPlugin({
      filename: 'assets/css/[name].[hash:7].css'
    }),
    // Compress extracted CSS. We are using this plugin so that possible
    // duplicated CSS from different components can be deduped.
    new OptimizeCSSPlugin({
      cssProcessorOptions: {
        safe: true
      }
    }),
    new webpack.optimize.CommonsChunkPlugin({
      name: 'vendor',
      minChunks: function(module, _) {
        return (
          module.resource &&
          /\.js$/.test(module.resource) &&
          module.resource.indexOf(path.join(__dirname, './node_modules')) === 0
        );
      }
    }),
    new webpack.optimize.CommonsChunkPlugin({
      name: 'manifest',
      chunks: ['vendor']
    }),
    new webpack.BannerPlugin({
      banner: `console.log("Last modification time: ${new Date().toLocaleString()}");`,
      raw: true,
      entryOnly: true,
      test: /\.(js|jsx|mjs)$/
    })
  ]);
}
