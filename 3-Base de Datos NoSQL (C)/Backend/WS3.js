var express = require('express')
var http = require('http')
var app = express()

var MongoClient = require('mongodb').MongoClient;

var url = "mongodb://127.0.0.1:27017";  
const dbName = 'GranjaUrbana';

app.get('/Registro', (req, res) => {
    
    MongoClient.connect(url, function(err, client) {  
        if (err) console.log('1',err);  
        
        const db = client.db(dbName);
        var results = db.collection('Registro').find().toArray((err, row) => {
          res.status(200).send(row)
      });
               
                
        client.close();  
        });  
})

app.get('/Registro/Planta/:Id', (req, res) => {
  MongoClient.connect(url, function(err, client) {  
    if (err) console.log('1',err);  
    var itemId = req.params.Id;
    const db = client.db(dbName);
    var results = db.collection('Registro').find({IdPlanta: itemId}).toArray((err, row) => {
      res.status(200).send(row)
  });
           
            
    client.close();  
    }); 
  })

  app.get('/Registro/IP/:Id', (req, res) => {
    MongoClient.connect(url, function(err, client) {  
      if (err) console.log('1',err);  
      var itemId = req.params.Id;
      const db = client.db(dbName);
      var results = db.collection('Registro').find({IP: itemId}).toArray((err, row) => {
        res.status(200).send(row)
    });
             
              
      client.close();  
      }); 
    })

http.createServer(app).listen(8001, () => {
  console.log('Server started at http://localhost:8001');
});