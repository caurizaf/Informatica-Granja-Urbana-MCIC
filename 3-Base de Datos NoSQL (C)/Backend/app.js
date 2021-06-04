const schedule = require('node-schedule')
const fs = require('fs')
var MongoClient = require('mongodb').MongoClient;

var url = "mongodb://127.0.0.1:27017";  
const dbName = 'GranjaUrbana';


/*const someDate = new Date('2021-06-02T20:00:00.000+5:30')
schedule.scheduleJob(some, () => {
    console.log('Job ran @', new Date().toString())
})*/

const mjob = schedule.scheduleJob('*/1 * * * *', () => {
    console.log(' Leyendo... ')
    
    var lineReader = require('readline').createInterface({
        input: require('fs').createReadStream('sensor.txt')
      });
      
      lineReader.on('line', function (line) {
        //console.log('Line from file:', line);
        var fields = line.split(";");
        console.log('Line from file:', fields[1]);

//Id;Date;IdPlanta1;Higrometro1(%);Temp1;IdPlanta2;Higrometro2(%);Temp2;Latitud;Longitud;Altitud;MAC;IP

//var id = 1;
        MongoClient.connect(url, function(err, client) {  
            if (err) console.log('1',err);  
            var myobj = [     
                { "Date": fields[1], "IdPlanta": fields[2], "Humedad": fields[3], "Temperatura": fields[4], "Latitud": fields[8], "Longitud": fields[9], "Altitud": fields[10], "MAC": fields[12], "IP": fields[13]},  
                { "Date": fields[1], "IdPlanta": fields[5], "Humedad": fields[6], "Temperatura": fields[7], "Latitud": fields[8], "Longitud": fields[9], "Altitud": fields[10], "MAC": fields[12], "IP": fields[13]} 
                ];    
            const db = client.db(dbName);
            db.collection('Registro').insertMany(myobj, function(err, res) {  
            if (err) console.log('2',err);  
            console.log("Number of records inserted: " + res.insertedCount);  
            //client.close();  
            });  
            });  

      });

    // fs.readFile('sensor.txt', 'utf8' , (err, data) => {
    //     if (err) {
    //     console.error(err)
    //     return
    //     }
    //     console.log(data)
    // })

    mjob.cancel()
})


