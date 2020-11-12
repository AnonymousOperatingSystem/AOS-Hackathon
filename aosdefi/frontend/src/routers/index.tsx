import React from "react";
import { Router, Route, Redirect, Switch } from "react-router-dom";
import App from "../App";
import history from "./history";

export default () => {
  return (
    <div className="root">
      <Router history={history}>
        <Switch>
          <Route path="/" exact component={App} />

          <Redirect to="/" />
        </Switch>
      </Router>
    </div>
  );
};
