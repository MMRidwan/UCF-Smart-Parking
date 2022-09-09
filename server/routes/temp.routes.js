const express = require("express");
const router = express.Router();
const temp = require("../controllers/temp.controller");

// Login
router.get("/login", temp.login);

module.exports = router;