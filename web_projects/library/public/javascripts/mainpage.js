
const inputs = document.querySelectorAll('.popup-input');
let uploadImg = document.getElementById('upload-image');
const defaultImgSrc = "../images/book-images/default_cover.png";
const chooseImgSrc = "../images/book-images/default_cover.png";
const uploadBtn = document.getElementById("uploading");
let imgSrc = defaultImgSrc;

let colBtn = document.getElementById('allbooksBtn');
document.getElementById('addBtn').onclick = add_popup;
document.getElementById('returnBtn').onclick = return_popup();
//document.getElementById('delBtnBtn').onclick = del_popup();

function jump_to_page()
{
    colBtn = document.getElementById("TablePage");
    colBtn.scrollIntoView({behavior: "smooth", block:"start"});
}


const closePopup = () => {
    inputs.forEach(v => v.value = '');

    imgSrc = defaultImgSrc;
    // console.log(imgSrc);
    uploadImg = chooseImgSrc;
    add_popup.classList.remove('show');

}

function add_popup()
{
    document.getElementById("addForm").style.display = "block";
}

function fetching_data(){
    let book = JSON.stringify({
        name: inputs[0].value,
        author: inputs[1].value,
        year: inputs[2].value,
        stock: "available",
        renter: '',
        image: imgSrc,
        id: (parseInt(library[library.length - 1].id, 10) + 1).toString()})
    console.log('book', book );
    // id: (parseInt(library.find.id, 10) + 1).toString()});
    console.log((parseInt(library[library.length - 1].id, 10) + 1).toString())
    let form = new FormData();
    form.append('book', book);
    fetch('/uploadBook', {
        method: 'POST',
        body: form
    })
        .then(() => {
            postLibrary();
            closePopup();
        });

}

function uploadImage(){
    let form = new FormData();
    window.addEventListener('load', function() {
        document.querySelector('input[type="file"]').addEventListener('change', function() {
            if (this.files && this.files[0]) {
                let  img = document.querySelector('img');
                img.onload = () => {
                    URL.revokeObjectURL(img.src);
                }
                img.src = URL.createObjectURL(this.files[0]);
                form.append('usrImage', files[0]);
            }
        });
    });
    console.log(imgSrc)
    fetch('/saveImage', {
        method: 'POST',
        body: form
    })
        .then(response => response.json())
        .then(data => {
            uploadImg.image = data.path;
            imgSrc = data.path;
        })
        .catch(err => {
            console.error(err);
        })
}

function return_popup()
{
    
}

function closeForm() {
    document.getElementById("addForm").style.display = "none";
}
function closeDel() {
    document.getElementById("delPop").style.display = "none";
}