from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Show current user's statistics"""
    
    # get current user information
    user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])

    # get grouped transactions
    transactions = db.execute("SELECT username, stock_symbol, SUM(shares) FROM transactions WHERE username = :username GROUP BY stock_symbol ORDER BY stock_symbol", username=user[0]["username"])

    # prepare data for display
    data = []
    total_shares_price = 0
    
    for transaction in transactions:
        stock_data = lookup(transaction["stock_symbol"])
        shares = transaction["SUM(shares)"]
        price = round(stock_data["price"], 2)
        total_shares_price = total_shares_price + (shares * price)
        
        if shares > 0:
            data.append({"symbol": transaction["stock_symbol"], "name": stock_data["name"], "shares": shares, "price": price, "total": round((shares * price), 2)})
        
    return render_template("index.html", data=data, username=user[0]["username"], cash=round(user[0]["cash"], 2), grand_total=round(user[0]["cash"] + total_shares_price, 2))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure stock symbol was submitted
        if not request.form.get("stock_symbol"):
            return apology("must provide stock symbol")
        
        # ensure number of shares was submitted
        if not request.form.get("shares_number"):
            return apology("must provide number of shares")
        
        # save number of shares in a variable
        shares = int(request.form.get("shares_number"))

        # ensure number of shares is higher than zero
        if shares < 1:
            return apology("must buy more than zero shares")
        
        # get stock data
        stock_data = lookup(request.form.get("stock_symbol"))

        # check returned stock data and show "apology" page if there is a mistake
        if not stock_data:
            return apology("there was an error with your request")
            
        # make money calculations
        stock_price = stock_data["price"] * shares
        user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        user_cash = user[0]["cash"]
        cash_left = user_cash - stock_price
        
        # check if user has enough money to buy the required number of shares
        if cash_left < 0:
            return apology("{} stocks cost {}, you have {}".format(shares, round(stock_price, 2), round(user_cash, 2)))
        
        # save purchased stock
        today = datetime.now();
        db.execute("INSERT INTO transactions (username, stock_symbol, shares, purchase_date, price) VALUES(:username, :stock_symbol, :shares, :purchase_date, :price)", username=user[0]["username"], stock_symbol=stock_data["symbol"], shares=shares, purchase_date=today, price=(stock_price * -1))

        # update user's cash after purchase 
        db.execute("UPDATE users SET cash = :cash_left WHERE id = :id", cash_left=cash_left, id=session["user_id"])

        # after a successful purchase redirect user to index page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")
        
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # if user reached route via POST (as by submitting a quote via POST)
    if request.method == "POST":
        
        # ensure stock symbol was submitted
        if not request.form.get("stock_symbol"):
            return apology("must provide stock symbol")
        
        # get stock data
        stock_data = lookup(request.form.get("stock_symbol"))
        
        # check returned stock data and show "apology" page if there is a mistake
        if not stock_data:
            return apology("there was an error with your request")
        
        # display "quoted" page with stock data
        return render_template("quoted.html", data=stock_data)

    # if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
        
        # ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password")
            
        # ensure password confirmation was submitted
        if not request.form.get("password_confirmation"):
            return apology("must provide password confirmation")
        
        # ensure password confirmation is the same as password
        if request.form.get("password") != request.form.get("password_confirmation"):
            return apology("password confirmation must be the same as password")
        
        # hash password
        hashed_password = pwd_context.hash(request.form.get("password"))
        
        # add username and password to the database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hashed_password)
        
        # ensure the provided username is not already taken
        if not result:
            return apology("this username is already taken")
        
        # remember user
        session["user_id"] = result
            
        # redirect user to home page
        return redirect(url_for("index"))
        
    # if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # get stock symbols from submitted form
        stock_symbols = request.form
        
        # get current cash from user account
        user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        current_cash = user[0]["cash"]

        received_cash = 0

        for stock_symbol in stock_symbols:
            # get number of sold shares
            sold_shares = int(request.form.get(stock_symbol))
            
            if sold_shares > 0:
                # calculate how much cash user receives for the sold shares
                stock_data = lookup(stock_symbol)
                received_cash = received_cash + (stock_data["price"] * sold_shares)
                # add sale transaction
                today = datetime.now();
                db.execute("INSERT INTO transactions (username, stock_symbol, shares, purchase_date, price) VALUES(:username, :stock_symbol, :shares, :purchase_date, :price)", username=user[0]["username"], stock_symbol=stock_symbol, shares=(sold_shares * -1), purchase_date=today, price=(stock_data["price"] * sold_shares))

        if received_cash > 0:
            # update cash in user account
            db.execute("UPDATE users SET cash = :new_cash WHERE id = :id", new_cash=(current_cash + received_cash), id=session["user_id"])

        return redirect(url_for("index"))
    
    # if user reached route via GET (as by clicking a link or via redirect)
    else:
        # get current user information
        user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
    
        # get grouped transactions
        transactions = db.execute("SELECT stock_symbol, SUM(shares) FROM transactions WHERE username = :username GROUP BY stock_symbol ORDER BY stock_symbol", username=user[0]["username"])
    
        # prepare data for display
        data = []

        for transaction in transactions:
            stock_data = lookup(transaction["stock_symbol"])
            shares = transaction["SUM(shares)"]
            price = round(stock_data["price"], 2)

            if shares > 0:
                data.append({"symbol": transaction["stock_symbol"], "name": stock_data["name"], "shares": shares, "price": price })
            
        return render_template("sell.html", data=data)
