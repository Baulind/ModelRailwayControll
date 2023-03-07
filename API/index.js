"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
// Import the express in typescript file
const express_1 = __importDefault(require("express"));
const sqlite3_1 = __importDefault(require("sqlite3"));
// Initialize the express engine
// Take a port 3000 for running server.
const app = (0, express_1.default)();
const port = 3000;
//Esatblish connection with database
const db = new sqlite3_1.default.Database('..\\Database\\db.sqlite', (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the database.');
});
app.get('/', (_req, _res) => {
    console.log("root");
    return _res.send("Hello World!");
});
// Handling '/api/values' Request
app.get('/api/values', (_req, _res) => {
    _res.set('Access-Control-Allow-Origin', '*');
    let sql = "SELECT * FROM motorValue";
    let params = [];
    console.log("Hit!");
    db.all(sql, params, (err, rows) => {
        if (err) {
            _res.status(400).json({ "error": err.message });
            return;
        }
        let temp = { "message": "success", "data": rows };
        _res.json(temp);
    });
});
// Server setup
app.listen(port, () => {
    console.log(`TypeScript with Express http://localhost:${port}/`);
});
