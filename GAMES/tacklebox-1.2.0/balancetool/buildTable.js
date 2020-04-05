function buildHabitatTd(hour, habitat) {
    const baitObject = hour[habitat];
    const body = Object.keys(baitObject).reduce((str, baitName) => {
        if (baitObject[baitName] && baitObject[baitName].count) {
            return str + baitName[0] + baitObject[baitName].ratio + " ";
        }
        return str;
    }, "");

    return `<td>${body}</td>`;
}

function buildRow(habitats, hour) {
    const habitatTds = habitats.map(buildHabitatTd.bind(null, hour));
    return `
<tr><td>${hour.hour}</td>${habitatTds.join("")}</tr>`;
}

module.exports = function buildTable(hours, habitats) {
    const rows = hours.map(buildRow.bind(null, habitats));

    return `
<!doctype html>
<html>
<head>
    <title>fish balance report</title>
</head>
<body>
    <h1>fish balance report</h1>
    <table border="1">
        <thead>
            <td>hour</td>
            ${habitats.map(h => `<td>${h}</td>`).join("")}
        </thead>
        <tbody>
        ${rows.join("")}
        </tbody>
    </table>
</body>
</html>`;
};
