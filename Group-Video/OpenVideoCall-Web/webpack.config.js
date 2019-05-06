const path = require('path');
const webpack = require('webpack');
const UglifyJsPlugin = require('uglifyjs-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const OptimizeCSSAssetsPlugin = require('optimize-css-assets-webpack-plugin');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const CleanWebpackPlugin = require('clean-webpack-plugin');
const TerserPlugin = require('terser-webpack-plugin')
// const BundleAnalyzerPlugin = require('webpack-bundle-analyzer').BundleAnalyzerPlugin;
const pkg = require('./package.json');

const utils = require('./webpack.utils');

module.exports = {
  mode: process.env.NODE_ENV === 'production' ? 'production' : 'development',
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
        use: [
          {
            loader: require.resolve('html-loader'),
            options: {
              minimize: true,
            }
          }
        ]
      },
      {
        test: /\.(js|jsx|mjs)$/,
        include: path.resolve(__dirname, 'src'),
        use: [
          {
            loader: require.resolve('babel-loader'),
            options: {
              cacheDirectory: true
            }
          }
        ]
      },
      {
        test: [/\.(bmp|gif|jpe?g|png|svg)(\?.*)?$/],
        use: [
          {
            loader: require.resolve('url-loader'),
            options: {
              limit: 8000,
              name: 'assets/img/[name].[hash:7].[ext]'
            }
          }
        ]
      },
      {
        test: /\.(mp4|webm|ogg|mp3|wav|flac|aac)(\?.*)?$/,
        use: [
          {
            loader: require.resolve('url-loader'),
            options: {
              limit: 8000,
              name: 'assets/media/[name].[hash:7].[ext]'
            }
          }
        ]
      },
      {
        test: /\.(woff2?|eot|ttf|otf)(\?.*)?$/,
        use: [
          {
            loader: require.resolve('url-loader'),
            options: {
              limit: 8000,
              name: 'assets/fonts/[name].[hash:7].[ext]'
            }
          }
        ]
      }
    ])
  },
  resolve: {
    alias: {
      '@': path.join(__dirname, './src'),
      // 'jquery': 'jquery/dist/jquery.slim.js'
    },
    extensions: ['*', '.js', '.json']
  },
  plugins: utils.getHtml().concat([
    new CopyWebpackPlugin([
      {
        from: path.resolve(__dirname, './static'),
        to: 'assets/',
        ignore: ['.*']
      }
    ]),
    new webpack.DefinePlugin({
      agoraVersion: JSON.stringify(pkg.dependencies['agora-rtc-sdk'])
    })
    // new BundleAnalyzerPlugin()
  ])
};

if (process.env.NODE_ENV === 'production') {
  module.exports.output.publicPath = './';
  module.exports.optimization = {
    splitChunks: {
      cacheGroups: {
        vendor: {
          test: /node_modules/,
          chunks: 'all',
          name: 'vendor'
        }
      }
    },
    minimizer: [
      // new UglifyJsPlugin({
      //   cache: true,
      //   parallel: true,
      //   sourceMap: true // Set to true if you want JS source maps
      // }),
      new TerserPlugin({
        parallel: true,
        terserOptions: {
          ecma: 6
        }
      }),
      new OptimizeCSSAssetsPlugin({})
    ]
  };
  module.exports.plugins = (module.exports.plugins || []).concat([
    new CleanWebpackPlugin(),
    // Extract css into its own file
    new MiniCssExtractPlugin({
      filename: 'assets/css/[name].css'
    }),
    new webpack.BannerPlugin({
      banner: `console.log("Last modification time: ${new Date().toLocaleString()}");`,
      raw: true,
      entryOnly: true,
      test: /\.(js|jsx|mjs)$/
    })
  ]);
}