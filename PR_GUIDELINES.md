# Pull Request Guidelines 📝

Simple rules to keep our PRs clean and easy to review!

## Daily Workflow 🌿

**Every day, follow these steps:**

1. **Start fresh - Pull the latest `development` branch:**
   ```bash
   git checkout development
   git pull origin development
   ```

2. **Create your branch:**

   - Select your branch type from the following:

   feature/ - Working on a new feature
   Example: feature/recall-insights

   bugfix/ - Bug fixes
   Example: bugfix/payment-gateway-timeout

   release/ - Preparing a new release
   Example: release/v1.2.0

   chore/ - Maintenance tasks
   Example: chore/cleanup-unused-code

   refactor/ - Code refactoring
   Example: refactor/extract-auth-service

   docs/ - Documentation updates
   Example: docs/update-api-docs 

   test/ - Adding or improving tests
   Example: test/add-coverage-for-auth 

   perf/ - Performance improvements
   Example: perf/reduce-bundle-size

   ```bash
   git checkout -b feature/your-feature-name
   ```
   - Use descriptive names: `feature/audiorecording`, `bugfix/cors-error`, etc.

3. **Make your changes** - Write your code, test it locally

4. **Commit your changes:**
   ```bash
   git add .
   git commit -m "Clear description of what you did"
   ```

5. **Push your branch:**
   ```bash
   git push -u origin feature/your-feature-name
   ```

6. **Create a Pull Request** - Go to GitHub and create a PR using the template
   - PR will automatically target `development` branch
   - Fill out the PR template completely
   - Wait for review and approval

7. **After PR is merged** - Clean up:
   ```bash
   git checkout development
   git pull origin development
   git branch -d feature/your-feature-name  # Delete local branch
   ```

## Before Creating a PR ✋

1. **Make sure your code works locally** - Test it! Run the app and make sure nothing breaks.
2. **Follow the project structure**:
   - **Frontend**: Put files in the right folders (`styles/`, `views/`, `components/`, `hooks/`, `context/`)
   - **Backend**: Use proper folders (`routes/`, `controllers/`, `services/`, `utils/`)
3. **Clean up your code**:
   - Remove any `console.log()` statements
   - Only leave minimal comments in code
   - Remove debug code

## PR Title & Description 📋

- **Keep titles clear and descriptive** (e.g., "Fix audio recording bug" not "fix stuff")
- **Fill out the PR template** - It helps reviewers understand what you changed and why

## What Makes a Good PR? ✅

- ✅ **Small and focused** - One feature or bug fix per PR is easier to review
- ✅ **Clear description** - Explain what you did and why
- ✅ **Working code** - Make sure it actually works before requesting review
- ✅ **Clean code** - No debug code or console logs

## Code Review 🧐

- Be open to feedback - We're all learning!
- Respond to comments promptly
- If something isn't clear, ask questions
- Don't take feedback personally - we're all on the same team 😊

## Questions? 💬

If you're not sure about something, just ask! Better to ask than to guess.

