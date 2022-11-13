const sorts = document.querySelector('.sort-content').querySelectorAll('a');
const sortBtn = document.getElementById('sortBtn');
sorts.forEach(a => a.onclick = sortLibrary)

function sortLibrary(e) {
    if (this === sorts[0]){
        library.sort((a, b) => {
            let  nameA = a.name.toLowerCase(), nameB = b.name.toLowerCase();
            if (nameA > nameB) return 1;
            else if (nameA < nameB) return -1;
            return 0;
        })
    }
    else if (this === sorts[1]){
        library.sort((a, b) => {
            return a.author.localeCompare(b.author);
        })
    }
    else if (this === sorts[2]) {
        library.sort((a, b) => {
            dateA = new Date(a.year)
            dateB = new Date(b.year)
            if (dateA > dateB) return 1;
            else if (dateA < dateB) return -1;
            return 0;
        })
    }
    else if(this === sorts[3]){
        if (availableFlag)
            availableFlag = false;
        else
            availableFlag = true;
    }
    renderLibrary();
    // library.sort((a, b) => {
    //     let  aid = a.id, bid = b.id;
    //     return aid.localeCompare(bid);
    // })


}