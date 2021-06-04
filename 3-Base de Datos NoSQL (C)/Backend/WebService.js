var express 	= require("express"); 
var app 	= express();
var bodyParser 	= require('body-parser');
var request 	= require("request");

var url 	= "https://jsonplaceholder.typicode.com/todos/"

//console.log("si corre")

app.use(bodyParser.json());
// Soporte para bodies codificados
app.use(bodyParser.urlencoded({ extended: true })); 
 
// Ejemplo: GET http://localhost:8080/users
// Consumimos datos Facke de la URL: https://jsonplaceholder.typicode.com/todos
app.get('/users', function(req, res) {
	request({
	    url: url,
	    json: false
	}, function (error, response, body) {

	    if (!error && response.statusCode === 200) {
	    	// Pintamos la respuesta JSON en navegador.
	        res.send(body) 
	    }
	})
});
 
//Ejemplo: GET http://localhost:8080/items/3
app.get('/users/:id', function(req, res) {

	var itemId = req.params.id;

	request({
	    url: url+itemId,
	    json: false
	}, function (error, response, body) {

	    if (!error && response.statusCode === 200) {
	    	// Pintamos la respuesta JSON en navegador.
	        res.send(body) 
	    }
	})
})
  
var server = app.listen(8888, function () {
    console.log('Server is running..'); 
});