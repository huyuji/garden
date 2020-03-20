// curl https://api.todoist.com/sync/v8/backups/get -d token=
// curl -L -H "Authorization: Bearer 0123456789abcdef0123456789abcdef01234567" https://downloads.todoist.com/12345678901234567890123456789012.zip

const token = '';
const backupFolder = ''

const urlGetBackups = 'https://api.todoist.com/sync/v8/backups/get';

const https = require('https');
const fs = require('fs');
const { exec } = require("child_process");

const data = JSON.stringify({
  token: token
});

const options = {
  hostname: 'api.todoist.com',
  port: 443,
  path: '/sync/v8/backups/get',
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
    'Content-Length': data.length
  }
};

request(options, processBackupList);

function downloadBackup(url, name)
{
    const command = `curl -L -H "Authorization: Bearer ${token}" ${url} --output ${backupFolder}/${name}`;
    exec(command, (error) => {
        if (error) {
            console.log(`error: ${error}`);
        } else {
            console.log(`success`);
        }
    });
}

function processBackupList(data) {
    backups = JSON.parse(data);
    backups.sort(function (a, b) {
        return Date.parse(b.version) -  Date.parse(a.version);
    });
    lastBackup = backups[0];
    backupName = lastBackup.version.substr(0, 10) + '.zip';
    backupUrl = lastBackup.url;

    downloadBackup(backupUrl, backupName);
}

function request(options, onLoad) {
    const req = https.request(options, (response) => {
        if (response.statusCode !== 200) {
            console.log(`statusCode: ${response.statusCode}`);
        } else {
            response.on('data', onLoad);
        }        
    });
    req.on('error', (error) => {
        console.log(`error: ${error}`);
    })
    req.write(data)
    req.end()
}
