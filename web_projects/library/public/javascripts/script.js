let library = ''
let availableFlag = false;
let createBookEl = (Obj) => {
    let placeholder = document.querySelector("#data-output");
    let out = "";
    for(let book of library){
        out += `
         <div id="inner">
            <a href='/books/${book.id}'>
            <img src='${book.image}' id ="upload-image"></a>
            <ul>
                <li>${book.name}</li>
                <li>${book.author}</li>               
            </ul>
         </div> 
      `;
    }
    placeholder.innerHTML = out;
}

const renderLibrary = () => {
    document.getElementById('data-output').innerHTML = '';
    if (availableFlag) {
        library.forEach((Obj) => {
        let stock = 'available';
        console.log(Obj.stock, stock, Obj.stock === stock);
        if (Obj.stock === stock)
        {
            createBookEl(library);
        }
        })
    }
    else {
        library.forEach((Obj) => {
                createBookEl(library);
        })
    }

}

const postLibrary = () => {
    fetch("/jsondata", {method: "POST"})
        .then((res) =>{
            return res.json();
        })
        .then((res) => {
            library = res;
            renderLibrary();
        })
        .catch((err) =>{
            console.log(err);
        })
}

postLibrary();