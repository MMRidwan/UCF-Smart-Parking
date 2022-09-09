const jwt = require("jsonwebtoken");

//rotating keys tokens - look into it
module.exports = function authenticateToken(req, res, next) {
  const authHeader = req.headers["authorization"];
  const token = authHeader && authHeader.split(" ")[1];
  if (token == null) return res.status(401).send();
  jwt.verify(token, process.env.AUTH_TOKEN_SECRET, (err, email) => {
    if (err) console.log(`token: ${token}`);
    if (err) return res.status(403).send();
    req.email = email.toLowerCase();
    next();
  });
};