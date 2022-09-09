const express = require("express");
const app = express();
const cors = require("cors");
const mongoose = require("mongoose");
const path = require("path");
require("dotenv").config();

const server = require("http").Server(app);

// Middleware
app.use(express.json());
app.use(cors());

if (process.env.NODE_ENV === "production") {
  app.use("/", express.static(path.resolve("client/build")));
  app.get(/^\/(?!api).*$/, function (req, res) {
    res.sendFile(path.join(path.resolve("client/build"), "index.html"));
  });
}
app.use("/api/temp", require("./routes/temp.routes.js"));

connect();

function listen() {
  const port = process.env.PORT || 5000;

  server.listen(port, () => {
    console.log(`Server started on port ${port}`);
  });
}
function connect() {
  mongoose.connection
    .on("error", console.log)
    .on("disconnected", connect)
    .once("open", listen);
  return mongoose.connect(process.env.DB, {
    keepAlive: 1,
    useNewUrlParser: true,
    useUnifiedTopology: true,
  });
}