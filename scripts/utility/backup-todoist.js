// curl https://api.todoist.com/sync/v8/backups/get -d token=
// curl -L -H "Authorization: Bearer 0123456789abcdef0123456789abcdef01234567" https://downloads.todoist.com/12345678901234567890123456789012.zip

const https = require('https');

const token = '';
const urlGetBackups = 'https://api.todoist.com/sync/v8/backups/get';
https.get(urlGetBackups, (response) => {
    
});