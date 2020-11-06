import React from "react";
import { BrowserRouter as Router, Switch, Route } from "react-router-dom";
import Home from "../page/Home";
import Rule from "../page/Rule";
import Record from "../page/Record";

export default function AppRouter() {
  return (
    <Router basename="/animal/">
      <Switch>
        <Route exact path="/" children={<Home />} />
        <Route path="/rule" children={<Rule />} />
        <Route path="/record" children={<Record />} />
      </Switch>
    </Router>
  );
}
