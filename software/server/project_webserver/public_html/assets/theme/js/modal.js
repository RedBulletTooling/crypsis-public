$(".btn[data-target='#myModal']").click(function() {
    console.log("hit")
    var columnHeadings = $("thead th").map(function() {
              return $(this).text();
           }).get();
    columnHeadings.pop();
    var columnValues = $(this).parent().siblings().map(function() {
              return $(this).text();
    }).get();
    var modalBody = $('<div id="modalContent"></div>');
    var modalForm = $('<form role="form" name="modalForm1" action="/api/update/config" method="post"></form>');
    $.each(columnHeadings, function(i, columnHeader) {
        var formGroup = $('<div class="form-group"></div>');
        formGroup.append('<label for="'+columnHeader+'">'+columnHeader+'</label>');
        formGroup.append('<input class="form-control" name="'+columnHeader+'" id="'+columnHeader+'" value="'+columnValues[i]+'" />');
        if (columnHeader == "Implant ID") {
            formGroup.append('<input type="hidden" class="form-control" name="current '+columnHeader+'" id="current '+columnHeader+'" value="'+columnValues[i]+'" readonly/>');
        } 
        modalForm.append(formGroup);
    });
    modalBody.append(modalForm);
    $('.modal-body').html(modalBody);
    });
    $('.modal-footer .btn-outline-primary').click(function() {
    $('form[name="modalForm1"]').submit();
});

$(".btn[data-target='#deletemodal']").click(function() {
    console.log("hit")
    var columnHeadings = $("thead th").map(function() {
              return $(this).text();
           }).get();
    columnHeadings.pop();
    var columnValues = $(this).parent().siblings().map(function() {
              return $(this).text();
    }).get();
    var modalBody = $('<div id="modalContent"></div>');
    var modalText = $('<b>Are you sure you want to delete this implant?</b>')
    var modalForm = $('<form role="form" name="modalForm2" action="/api/delete/config" method="post"></form>');
    $.each(columnHeadings, function(i, columnHeader) {
        var formGroup = $('<div class="form-group"></div>');
        formGroup.append('<label for="'+columnHeader+'">'+columnHeader+'</label>');
        formGroup.append('<input class="form-control" name="'+columnHeader+'" id="'+columnHeader+'" value="'+columnValues[i]+'" readonly/>');
        modalForm.append(formGroup);
    });
    modalBody.append(modalForm);
    modalBody.append(modalText);
    $('.modal-body2').html(modalBody);
    });
    $('.modal-footer .btn-outline-danger').click(function() {
    $('form[name="modalForm2"]').submit();
});

$('.modal-footer .btn-outline-success').click(function() {
    $('form[name="modalForm3"]').submit();
    });

$('.modal-footer .btn-outline-primary').click(function() {
    $('form[name="modalForm4"]').submit();
});

$('.modal-footer .btn-outline-primary').click(function() {
    $('form[name="modalForm5"]').submit();
});

var ele = document.getElementById('drop');
$.getJSON('/api/get/config', function (data) {
    for (var i = 0; i < data.length; i++) {
        // POPULATE SELECT ELEMENT WITH JSON.
        ele.innerHTML = ele.innerHTML +
        //'<option value="' + birds[i]['ID'] + '">' + birds[i]['Bird_Name'] + '</option>';
        '<option>' + data[i]['Implant Project'] + ' - ' + data[i]['Implant ID'] + ' - ' + data[i]['Implant Location'] +'</option>'
    }
})