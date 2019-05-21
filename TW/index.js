const DB_FILE = "db.json"

const express = require("express")
const fs = require("fs")

let db = {}
try {
  db = JSON.parse(fs.readFileSync(DB_FILE))
} catch (e) {
}

const app = express()

app.use(require("body-parser").json())

app.get("/api/books", (req, res) => {
  res.json(db["books"])
})

app.post("/api/books", (req, res) => {
  db["books"].push(req.body)
  fs.writeFile(DB_FILE, JSON.stringify(db), () => {})
})

app.use(express.static("public"))

app.listen(3000, () => console.log("Server running on port 3000"))