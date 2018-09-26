import React, { Component } from 'react'
import { HashRouter as Router, Route } from 'react-router-dom'
import 'bulma/css/bulma.css'

import './App.css'
import Index from './index'
import Meeting from './meeting'

class App extends Component {
  render() {
    return (
      <Router>
        <div className="full">
          <Route exact path="/" component={Index} />
          <Route path="/meeting" component={Meeting} />
        </div>
      </Router>
    )
  }
}

export default App
