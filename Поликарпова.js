import React, { Component } from "react";
import Container from 'react-bootstrap/lib/Container'
import Col from 'react-bootstrap/lib/Col'
import Row from 'react-bootstrap/lib/Row'
import Button from 'react-bootstrap/lib/Button'
import Form from 'react-bootstrap/lib/Form'
import Tabs from 'react-bootstrap/lib/Tabs'
import Tab from 'react-bootstrap/lib/Tab'
import Table from 'react-bootstrap/lib/Table'

export class MySamples extends Component {
    constructor(props) {
        super(props);
		
		this.state = {
			tableBody: []
		}
	}
	
	componentDidMount() {
		//console.log("MS-componentDidMount");
		this.tick();
	}
	
	tick() {
		//console.log("MS-tick");
		
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
						<td>{sample.greenLines}</td>
						<td>{sample.redLines}</td>
					</tr>) );
			}
			
			this.setState( { tableBody: tableBody} );
		} ).catch(er => { console.log(error)});
	}
  
    render () {
		//console.log("MS-render");
		return (
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
					<tbody>
						{this.state.tableBody}
					</tbody>
				</Table>
			</Col></Row></Container>);
	}
}