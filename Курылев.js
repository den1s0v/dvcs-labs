import React, { Component } from "react";
import Container from 'react-bootstrap/lib/Container'
import Col from 'react-bootstrap/lib/Col'
import Row from 'react-bootstrap/lib/Row'
import Button from 'react-bootstrap/lib/Button'
import Form from 'react-bootstrap/lib/Form'
import Tabs from 'react-bootstrap/lib/Tabs'
import Tab from 'react-bootstrap/lib/Tab'
import Table from 'react-bootstrap/lib/Table'

import { LoginOrSignup } from "./LoginOrSignup";

export class Index extends Component {
    constructor(props) {
        super(props);
		
		this.state = {
			tableBody: []
		}
		
		this.reloadTable = this.reloadTable.bind(this);
	}
	
	addSample() {
		
		const title = document.getElementById("title").value;
		const answer = document.getElementById("answer").value;
		const greenLines = [];
		const redLines = [];
		
		let lines = document.getElementsByClassName("greenLine");
		for (const line of lines) {
			greenLines.push(line.value);
		}
		
		lines = document.getElementsByClassName("redLine");
		for (const line of lines) {
			redLines.push(line.value);
		}
		
		if ( title == "" || answer == "" || greenLines.length == 0 || redLines.length == 0 ) {
			
			alert("Enter all data for new sample");
			
		} else {
			fetch('/api/samples/new', {
									method: 'POST',
									body: JSON.stringify({title: title, answer: answer, greenLines: greenLines, redLines: redLines}),
									headers: new Headers({"Content-Type": "application/json", "authorization":localStorage.userToken})
								})
			alert("New sample added.");
		}
	}
	
	reloadTable(tab, e) {
		
		if (tab === "table") {
			
			//alert("update");
			let tableBody = [];
			
			fetch('/api/samples/getUserSamples', {
										method: 'POST',
										body: JSON.stringify({}),
										headers: new Headers({"Content-Type": "application/json", "authorization":localStorage.userToken})
			}).then(response => response.json()).then(samples => {
										
				for(const sample of samples) {
					
					tableBody.push( (<tr>
							<td>{sample.title}</td>
							<td>{sample.answer}</td>
							<td>First: {sample.greenLines[0]}<br/>Second: {sample.greenLines[1]}</td>
							<td>First: {sample.redLines[0]}<br/>Second: {sample.redLines[1]}</td>
						</tr>) );
				}
				
				this.setState( { tableBody: tableBody} );
			} );
		}
	}
	//Another commit to my branch
	logout() {
		
		localStorage.setItem("userToken", "null");
		location.reload(true);
	}

    render () {
		
		return localStorage.getItem("userToken") !== "null" ? ( 
		<div>
			<Tabs defaultActiveKey="newSample" transition={false} id="noanim-tab-example" onSelect={ (tab, e) => this.reloadTable(tab,e) }>
				<Tab eventKey="newSample" title="Add new sample">
					<Container><Row><Col md={{ span: 5, offset: 3}}>
						<br />
						<Form>
							<Form.Group>
								<Form.Label>Title</Form.Label>
								<Form.Control id="title" type="text" placeholder="Enter title" />	
							</Form.Group>
							
							<Form.Group>
								<Form.Label>Answer</Form.Label>
								<Form.Control id="answer" type="text" placeholder="Enter regexp" />	
							</Form.Group>
							
							<Form.Group>
								<Form.Label>Right string</Form.Label>
								<Form.Control className="greenLine" type="text"/>	
								<br />
								<Form.Control className="greenLine" type="text"/>	
							</Form.Group>
							
							<Form.Group>
								<Form.Label>Wrong string</Form.Label>
								<Form.Control className="redLine" type="text"/>	
								<br />
								<Form.Control className="redLine" type="text"/>	
							</Form.Group>
							
							<Container><Row><Col md={{ offset:4}}>
							<Button variant="success" onClick={this.addSample}>Add sample</Button>
							</Col></Row></Container>
						</Form>
					</Col></Row></Container>
				</Tab>
				<Tab eventKey="table" title="My samples">
					<Container><Row><Col md={{ span: 5, offset: 3}}>
						<br />

						<Table striped bordered hover variant="dark">
						
							<thead>
								<tr>
								<th>Title</th>
								<th>Answer</th>
								<th>Right lines</th>
								<th>Wrong lines</th>
								</tr>
							</thead>
							<tab>
							</tab>
							<tbody>
								{this.state.tableBody}
							</tbody>
						
						</Table>
					</Col></Row></Container>
				</Tab>
				<Tab eventKey="account" title="My account">
					<Container><Row><Col md={{ span: 5, offset: 3}}>
						<br />
						<Button variant="success" onClick={this.logout}>Log out</Button>
					</Col></Row></Container>
				</Tab>
			</Tabs>
		</div> ) : ( <div> <LoginOrSignup/> </div> );
	}
}