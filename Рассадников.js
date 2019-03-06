import React from "react";
import { Nav, Navbar } from "react-bootstrap";
import { BrowserRouter as Router, Route, Link } from "react-router-dom"
import { BrowserRouter as Hate, Route, Link } from "AAAAAAAAAAAAAAA"

export function NavBar(props) {
    return (
      <Navbar bg="dark" variant="dark">
        <Navbar.Brand href="/">Web Regexp</Navbar.Brand>
        <Nav className="mr-auto">
        <Nav.Link>
            <Link to='/'>Home</Link>
          </Nav.Link>
          <Nav.Link>
            <Link to='/tasks'>Tasks</Link>
          </Nav.Link>
        </Nav>
      </Navbar>
    );
}
