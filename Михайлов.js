export class LoginOrSignup extends Component {
    constructor(props) {
        super(props);
	}
	
	login() {
		
		const email = document.getElementById("emailInput").value;
		const password = document.getElementById("passwordInput").value;

		if ( email == "" || password == "" ) {
			
			alert("Enter email and password");
		} else {
		
			//alert("Log in...");
			fetch('/api/login', {
									method: 'POST',
									body: JSON.stringify({email: email, password: password}),
									headers: new Headers({"Content-Type": "application/json"})
								})
			.then(response => response.json()).then(userInfo => {
			
				if (userInfo.error) {
					
					alert(userInfo.error);
					
				} else {
					//alert("You're log in! :3")
					localStorage.setItem("userToken", userInfo.token);
					location.reload(true);
				}
			});
		}
	}
	
	signup() {
		
		const email = document.getElementById("emailSignup").value;
		const password = document.getElementById("passwordSignup").value;

		if ( email == "" || password == "" ) {
			
			alert("Enter email and password");
		} else {
		
			alert("Sign up...");
			fetch('/api/signup', {
									method: 'POST',
									body: JSON.stringify({email: email, password: password, isAdmin: false}),
									headers: new Headers({"Content-Type": "application/json"})
								})
			.then(response => response.json()).then(userInfo => {
			
				if (userInfo.error) {
					
					alert(userInfo.error);
					
				} else {
					alert("You're sign up!!! :3")
					localStorage.setItem("userToken", userInfo.token);
					location.reload(true);
				}
			});
		}
	}
	
    render () {
		
		return ( 
		<div>
			<br />
			
			<Container>
				<Row>
					<Col md={{span:4, offset:2}}>
						<Form>
						  <Form.Label>Log In</Form.Label>
						  <Form.Group controlId="email">
							<Form.Label>Email</Form.Label>
							<Form.Control id="emailInput" type="email" placeholder="Enter email" />	
						  </Form.Group>

						  <Form.Group controlId="password">
							<Form.Label>Password</Form.Label>
							<Form.Control id="passwordInput" type="password" placeholder="Password" />
						  </Form.Group>
						  
						  <Button variant="success" onClick={this.login}>
							Log In
						  </Button>
						</Form>
					</Col>
					
					<Col md={{span:4, offset:1}}>
						<Form>
						  <Form.Label>Sign Up</Form.Label>
						  <Form.Group controlId="email">
							<Form.Label>Email</Form.Label>
							<Form.Control id="emailSignup" type="email" placeholder="Enter email" />	
						  </Form.Group>

						  <Form.Group controlId="password">
							<Form.Label>Password</Form.Label>
							<Form.Control id="passwordSignup" type="password" placeholder="Password" />
						  </Form.Group>
						  
						  <Button variant="primary" onClick={this.signup}>
							Sign Up
						  </Button>
						</Form>
					</Col>
				</Row>
				
				<br />
				<br />
				<br />
				
				<Row>
					<Col></Col>
					<Col>
						<GoogleButton/>
					</Col>
					<Col></Col>
				</Row>
				{}
			</Container>
		</div> );
	}
}