import React, { Component } from "react";
import Container from 'react-bootstrap/lib/Container'
import Col from 'react-bootstrap/lib/Col'
import Row from 'react-bootstrap/lib/Row'
import Alert from 'react-bootstrap/lib/Alert'
import Button from 'react-bootstrap/lib/Button'
import ButtonToolbar from 'react-bootstrap/lib/ButtonToolbar';
import Form from 'react-bootstrap/lib/Form'

import { Redirect } from "react-router-dom"


export class Sample extends Component {
    constructor(props) {
        super(props);
		
		this.state = { 
			show: true,
			canSolve: true
		};
		
	}
	
	checkAnswer(event) {
		
		//console.log(event.target.parentNode.previousSibling.lastChild.value);
		const value = event.target.parentNode.previousSibling.lastChild.value;
		
		if ( this.state.canSolve ) {
			
			alert("You already solve this task");
		}
		else if ( value === this.props.sample.answer ) {
				
			//сохранить в у usera в список решенных
			
			fetch('/api/samples/addSolvedSample', {
										method: 'POST',
										body: JSON.stringify({ sampleId: this.props.sample._id}),
										headers: new Headers({"Content-Type": "application/json", "authorization":localStorage.userToken})
			});
			
			fetch('/api/samples/addSolvedUser', {
										method: 'POST',
										body: JSON.stringify({ sampleId: this.props.sample._id}),
										headers: new Headers({"Content-Type": "application/json", "authorization":localStorage.userToken})
			});
				
			this.setState( { canSolve:false } );
			
		} 
	}
	
    render () {
		
		const handleHide = () => this.setState({ show: false });
		const handleShow = () => this.setState({ show: true });
		return (
		  <div>
			<Alert show={this.state.show} variant="dark">
			  <Alert.Heading>{this.props.sample.title}</Alert.Heading>
				
				<Form>
					<Form.Label>Right lines:</Form.Label>
					<Form.Group>
						<Form.Label>
							&nbsp;{this.props.sample.greenLines[0]}<br />
							&nbsp;{this.props.sample.greenLines[1]}
						</Form.Label>
					</Form.Group>
					<Form.Label>Wrong lines:</Form.Label>
					<Form.Group>
						<Form.Label>
							&nbsp;{this.props.sample.redLines[0]}<br />
							&nbsp;{this.props.sample.redLines[1]}
						</Form.Label>
					</Form.Group>

					<Form.Group>
						<Form.Label>Answer</Form.Label>
						{ ( this.props.canSolve && this.state.canSolve ) && <Form.Control type="text" placeholder="You're answer" />}
						{ ( !this.props.canSolve || !this.state.canSolve ) && <Form.Control type="text" value={this.props.sample.answer} readOnly={true}/>}
					</Form.Group>

					<ButtonToolbar>
						{ ( this.props.canSolve && this.state.canSolve ) && <Button onClick={ (event) => this.checkAnswer(event) } variant="info">Check answer</Button>}
						&nbsp;&nbsp;&nbsp;<Button onClick={handleHide} variant="light">Close</Button>
					</ButtonToolbar>
				</Form>
				
			</Alert>

			{!this.state.show && <Button onClick={handleShow}>{this.props.sample.title}</Button>}
		  </div>
		);
	}
}