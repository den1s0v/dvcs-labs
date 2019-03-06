import React, { Component } from "react";
import { hot } from "react-hot-loader/root";

import { NavBar } from "./NavBar";
import { Route } from "react-router-dom"

import { Index } from "./Index";
import { Samples } from "./Samples";

class App extends Component {

  render() {
    return (
      <div>
        <NavBar></NavBar>
		<span> Тут что-то будет</span>
        <Route path="/" exact component={Index} />
        <Route path="/newUtlToTasks" component={Samples} />
      </div>
	  
	  
    )
  }
}

export default hot(App);