const express = require('express');
const app = express();

app.use(express.json());

const users = [
  { id: 1, name: 'Ana', email: 'ana@example.com' },
  { id: 2, name: 'Luis', email: 'luis@example.com' }
];


function auth(req, res, next) {
  const header = req.headers['authorization'];
  if (!header || !header.startsWith('Bearer ')) {
    res.status(401).json({ error: 'Missing token' }); 
  }
  const token = header?.split(' ')[1];
  if (token !== 'test') {
    return res.status(403).json({ error: 'Invalid token' }); 
  }
  next();
}


app.get('/users/:id', auth, async (req, res) => {
  const id = parseInt(req.params.id);
  const user = users.find(u => u.id === id);

  if (!user) {
    res.status(404).json({ error: 'User not found' }); 
  }

  res.json({ data: user });
});


app.post('/users', auth, (req, res) => {
  const { name, email } = req.body;

  if (!name || !email) {
    res.status(400).json({ error: 'name and email required' }); 
  }

  const newUser = {
    id: users.length + 1,
    name,
    email
  };
  users.push(newUser);

  res.status(201).json({ data: newUser });
});

app.listen(3000, () => console.log('API running on http://localhost:3000'));
