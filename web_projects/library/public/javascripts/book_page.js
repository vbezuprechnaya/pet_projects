const page_id = document.URL.split('/').pop();

const reneter_name = document.getElementsByName('renter_name');
const home = document.querySelector('a');
const editBtn = document.getElementById('editBtn');
const backBtn = document.getElementById('NavBar');
const cancelDelBtn = document.getElementById('cancel_del_btn');
const confirmDelBtn = document.getElementById('confirm_del_btn');
const del_pop = document.getElementById("del_pop");
const delBtn = document.getElementById("delBtn")
const confirmlend = document.getElementById('confirm_lend');
const inputs = document.querySelectorAll('input');
const statusLabel = document.getElementById('status_label');
let available = statusLabel.textContent.includes('available');
const confirmreturn = document.getElementById('confirm_ret');

document.getElementsByClassName("cancel").onclick = closeForm;
document.getElementById("retcl").onclick = closeRet;
const lendBtn = document.getElementById('giveBtn').onclick = give_Book;
document.getElementById('returnBtn').onclick = return_book;
let editMode = false;

backBtn.onclick = () => home;

const closePopup = () => {
    del_pop.classList.remove('show');
    del_pop.style.display = "none";
    delBtn.disabled = false;
}
cancelDelBtn.onclick = closePopup;

delBtn.addEventListener('click', () => {
    setTimeout(() => {
        if (!del_pop.classList.contains('show')) {
            del_pop.style.display = "block";
            delBtn.disabled = true;
        }
    }, 100);
});

document.addEventListener('click', (e) => {
    const isClosest = e.target.closest("#delPop")
    if (!isClosest && del_pop.classList.contains('show')) {
        closePopup();
    }
});

confirmDelBtn.onclick = () => {
    let book = JSON.stringify({
        id: page_id});
    let form = new FormData();
    form.append('book', book);
    fetch('/deleteBook', {
        method: 'POST',
        body: form
    })
        .then(() => {
            closePopup();
            home.click();
        });
};

function closeForm(){
    document.getElementById('give_pop').style.display = "none";
}

function give_Book() {
    document.getElementById('give_pop').style.display = "block";
    reneter_name[0].addEventListener('input', (e) => {
        if (!(e.target.value.length === 0)) {
            statusLabel.textContent = "Status:  "+'unavailable';
        }
        else {
            statusLabel.textContent = "Status:  "+'available';
        }
    })
        // for (let i = 0; i < inputs.length; i++) {
    //     if (i !== 2) {
    //         inputs[i].addEventListener('input', () => {
    //             let values = [];
    //             inputs.forEach(v => {
    //                 if (v.id !== 'renter_input')
    //                     values.push(v.value)
    //             });
    //             editBtn.disabled = values.includes('');
    //         })
    //     }
    // }
}
confirmlend.onclick = () => {
    const book = JSON.stringify({
        name: inputs[0].value,
        author: inputs[1].value,
        year: inputs[3].value,
        renter: reneter_name[0].innerHTML.toString() + 'Rue',
        stock: statusLabel.textContent.split(' ').pop(),
        id: page_id});
    let form = new FormData();
    form.append('book', book);
    fetch('/updateBook', {
        method: 'POST',
        body: form
    });
    closeForm();
}

let editor = () => {
    // Turn off editor, confirming changes
    if (editMode) {
        editMode = false
        editBtn.textContent = "edit";
        inputs.forEach(el => el.disabled = true);

        const book = JSON.stringify({
            name: inputs[0].value,
            author: inputs[1].value,
            year: inputs[3].value,
            renter: inputs[2].value,
            stock: statusLabel.textContent.split(' ').pop(),
            id: page_id});
        let form = new FormData();
        form.append('book', book);
        fetch('/updateBook', {
            method: 'POST',
            body: form
        });
    }
    else if (!editMode) {
        editMode = true
        editBtn.textContent = '✓'
        inputs.forEach(el => el.disabled = false);
    }
}
function closeRet(){
    document.getElementById('return_pop').style.display = "none";
}

function return_book(){
    console.log(inputs[4])
    document.getElementById('return_pop').style.display = "block";
    inputs[4].value = 'available'
}

confirmreturn.onclick = () => {
    const book = JSON.stringify({
        name: inputs[0].value,
        author: inputs[1].value,
        year: inputs[3].value,
        renter: '',
        stock: inputs[4].value,
        id: page_id});
    let form = new FormData();
    form.append('book', book);
    fetch('/updateBook', {
        method: 'POST',
        body: form
    });
    closeRet();
}

editBtn.onclick = editor;
