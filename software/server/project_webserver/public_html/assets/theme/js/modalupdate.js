$(".btn[data-target='#rawdata']").click(function() {
    console.log("initializing rawdata modal")
    var columnHeadings = $("thead th").map(function() {
                return $(this).text();
            }).get();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    var columnValues = $("tbody td button").parent().siblings().map(function() {
                return $(this).text();
    }).get();
    var modalBody = $('<div id="modalContent"></div>');
    var modalForm = $('<form role="form" name="modalForm4" action="/api/download/export" method="get"></form>');
    $.each(columnHeadings, function(i, columnHeader) {
        var formGroup = $('<div class="form-group"></div>');
        formGroup.append('<label for="'+columnHeader+'">'+columnHeader+'</label>');
        formGroup.append('<input class="form-control" name="'+columnHeader+'" id="'+columnHeader+'" value="'+columnValues[i]+'" readonly/>');
        modalForm.append(formGroup);
    });
    var formGroup = $('<div class="form-group"></div>');
    formGroup.append('<label for=Date range>Date range (UTC)</label>');
    formGroup.append('<input type="text" class="form-control" name="datetimes"/>');
    modalForm.append(formGroup);
    var formGroup = $('<div class="form-group"></div>');
    formGroup.append('<label for=Timestamps>Timestamps</label>');
    formGroup.append('<select class="form-control" name="type" id="type"><option>Without timestamps</option><option>With timestamps</option></select>');
    modalForm.append(formGroup);
    modalBody.append(modalForm);
    $('.modal-body4').html(modalBody);
    $(function() {
        $('input[name="datetimes"]').daterangepicker({
        timePicker: true,
        startDate: moment().startOf('hour'),
        endDate: moment().startOf('hour').add(32, 'hour'),
        locale: {
            format: 'DD/MM/YYYY hh:mm A'
        }
        });
    });
    console.log("initializing rawdata modal - DONE")
})

$(".btn[data-target='#commands']").click(function() {
    console.log("initializing commands modal")
    var columnHeadings = $("thead th").map(function() {
                return $(this).text();
            }).get();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    columnHeadings.pop();
    var columnValues = $(this).parent().siblings().map(function() {
                return $(this).text();
    }).get();
    var modalBody = $('<div id="modalContent"></div>');
    var modalForm = $('<form role="form" name="modalForm5" action="/api/commands" method="post"></form>');
    $.each(columnHeadings, function(i, columnHeader) {
        var formGroup = $('<div class="form-group"></div>');
        formGroup.append('<label for="'+columnHeader+'">'+columnHeader+'</label>');
        formGroup.append('<input class="form-control" name="'+columnHeader+'" id="'+columnHeader+'" value="'+columnValues[i]+'" readonly/>');
        modalForm.append(formGroup);
    });
    var formGroup = $('<div class="form-group"></div>');
    formGroup.append('<label for=Commands>Commands</label>');
    formGroup.append('<input class="form-control" type="submit" name="command" value="DUMPFULLSD"/>');
    formGroup.append('<hr/>');
    formGroup.append('<input class="form-control" type="submit" name="command" value="CLEARSD"/>');
    formGroup.append('<hr/>');
    formGroup.append('<input class="form-control" type="submit" name="command" value="PUSH_CONFIG"/>');
    formGroup.append('<hr/>');
    formGroup.append('<input class="form-control" type="submit" name="command" value="TOGGLEKEYBOARD"/>');
    formGroup.append('<hr/>');
    formGroup.append('<input class="form-control" name="sleep_time" id="sleep_time" placeholder="Time in minutes"></input>');
    formGroup.append('<input class="form-control" type="submit" name="command" value="SLEEP"/>');
    modalForm.append(formGroup);
    modalBody.append(modalForm);
    $('.modal-body5').html(modalBody);
    console.log("initializing commands modal - DONE")
})