const HtmlWebpackPlugin = require("html-webpack-plugin");
const CleanWebpackPlugin = require("clean-webpack-plugin");
const FriendlyErrorsPlugin = require("friendly-errors-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CopyPlugin = require('copy-webpack-plugin');
const path = require("path");
const resolve = path.resolve;
const webpack = require('webpack');
const NODE_ENV = process.env.NODE_ENV;
const copyPath = NODE_ENV == 'development' ? '../dist' : '../build';

const plugins = [
  new HtmlWebpackPlugin({
    filename: "index.html",
    template: path.resolve(__dirname, "../index.html"),
  }),
  new CleanWebpackPlugin(),
  new FriendlyErrorsPlugin({
    compilationSuccessInfo: {
      messages: ["You application is running here http://localhost:8080"],
      notes: ["Some additionnal notes to be displayed upon successful compilation"],
    },
    onErrors: function(severity, errors) {},
    clearConsole: true,
    additionalFormatters: [],
    additionalTransformers: [],
  }),
  new MiniCssExtractPlugin({
    filename: "[name].[hash].css",
    chunkFilename: "[id].css"
  }),
  new CopyPlugin([
    { from: resolve(__dirname, '../static'), to: resolve(__dirname, copyPath) },
  ]),
  new webpack.ProvidePlugin({
    $: "jquery",
    jQuery: "jquery",
    Popper: 'popper.js/dist/umd/popper',
  })
];

module.exports = plugins;
