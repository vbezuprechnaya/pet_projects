var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

module.exports = router;
const { nextTick } = require("process");

express = require("express")
fileupload = require("express-fileupload")
app = express()
fs = require('fs')

app.set('view engine', 'pug')

let rawdata = fs.readFileSync('books.json');
let library = JSON.parse(rawdata);


app.use(express.static(__dirname + '/public'), fileupload());

app.get('/', (req, res, next) => {
  res.sendFile(__dirname + '/public/index.html');
  next();
});

app.get('/books/:num', (req, res) => {
  book = library.find(b => b.id == req.params.num);
  console.log(req.params.num);
  res.render('mypage', {
    id: book.id,
    img: '../book-images/' + book.image,
    name: book.name,
    author: book.author,
    year: book.year,
    stock: "Status:  " + book.stock,
    renter: book.renter
    })
})

app.post('/jsondata', (req, res) => {
  res.json(library)
});

app.post('/uploadBook', (req, res) => {
  const book = JSON.parse(req.body.book);
  library.push(book);
  console.log(book)
  res.writeHead(200, {
    'Content-Type': 'application/message'
  });
  res.end('book added');
})


app.post('/deleteBook', (req, res) => {
  const book = JSON.parse(req.body.book);
  // let index = library.findIndex(b => b.id === book.id.toString());
  library.splice(book.id.toString() - 1, 1);
  res.end('book deleted');
})

app.post('/updateBook', (req, res) => {
  const book = JSON.parse(req.body.book);
  // index = library.findIndex(b => b.id === book.id.toString());
  library[book.id.toString() -1].name = book.name;
  library[book.id.toString() -1].author = book.author;
  library[book.id.toString() -1].year = book.year;
  library[book.id.toString() -1].stock = book.stock;
  library[book.id.toString() -1].renter = book.renter;
  res.end('book updated');
})

app.post('/saveImage', (req, res) => {
  const file = req.files.image;
  const filename = req.files.image.name;
  const path = __dirname + '/public/images/book-images/' + filename;
  file.mv(path, error => {
    if (error) {
      console.error(error);
      res.writeHead(500, {
        'Content-Type': 'application/json'
      })
      res.end(JSON.stringify({status: 'error', message: error}))
      return
    }
    res.writeHead(200, {
      'Content-Type': 'application/json'
    })
    res.end(JSON.stringify({ status: 'success', path: 'images/book-images/' + filename}))
  })
});

app.listen(3500);

// null > 0 -false
// null == 0 - false
// null >= 0 - true

// typeof typeof null -string