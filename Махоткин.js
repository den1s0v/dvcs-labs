import React, { Component } from "react";
import Container from 'react-bootstrap/lib/Container'
import Col from 'react-bootstrap/lib/Col'
import Row from 'react-bootstrap/lib/Row'
import Form from 'react-bootstrap/lib/Form'
import Button from 'react-bootstrap/lib/Button'

import { GoogleButton } from "./social-buttons/GoogleButton";
import { VkButton } from "./social-buttons/VkButton";

export class Login extends Component {
    constructor(props) {
        super(props);
	}
	
	login() {
		
		const email = document.getElementById("emailInput").value;
		const password = document.getElementById("passwordInput").value;

		if ( email == "" || password == "" ) {
			
			alert("Enter email and password");
		} else {
		
			alert("Log in...");
			fetch('/api/login', {
									method: 'POST',
									body: JSON.stringify({email: email, password: password}),
									headers: new Headers({"Content-Type": "application/json"})
								})
			.then(response => response.json()).then(userInfo => {
			
				if (userInfo.error) {
					
					alert(userInfo.error);
					
				} else {
					alert("You're log in! :3")
					window.localStorage.setItem("userToken", JSON.stringify(userInfo.token));
					console.log(window.localStorage.getItem("userToken"));
//					console.log(JSON.stringify(userInfo, null, 2));

					console.log();
					// Mahotkin branch commit 2
				}
			});
		}
	}

    render() {
		return (
			<div>
				<Container>
					
					<Form>
					  <Form.Group controlId="email">
						<Form.Label>Email</Form.Label>
						<Form.Control id="emailInput" type="email" placeholder="Enter email" />	
					  </Form.Group>

					  <Form.Group controlId="password">
						<Form.Label>Password</Form.Label>
						<Form.Control id="passwordInput" type="password" placeholder="Password" />
					  </Form.Group>
					  
					  <Button variant="primary" onClick={this.login}>
						Submit
					  </Button>
					</Form>
					
					
					<Row>
						<Col></Col>
						<Col>
							<GoogleButton/>
						</Col>
						<Col></Col>
					</Row>
					<Row>
						<Col></Col>
						<Col>
							<VkButton/>
						</Col>
						<Col></Col>
					</Row>
				</Container>
			</div>
		);
	}
}
