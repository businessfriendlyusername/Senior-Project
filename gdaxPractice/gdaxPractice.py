import GDAX
import psycopg2
import secret


def main():
    #Define our connection string
    conn_string = secret.secret

    pc = GDAX.PublicClient(product_id="BTC-USD")

    history = pc.getProduct24HrStats()
    date = pc.getTime()

    #Get a connection
    try:
        conn = psycopg2.connect(conn_string);
    except: print("Failed")

    #Returns Cursor to perform queries
    cursor = conn.cursor()

    #Query to import historical records to database
    #Set up insert string
    toInsert = "INSERT INTO candle VALUES ('%s','%f','%f','%f','%f','5','b');" % (date['iso'], float(history['open']), float(history['last']), float(history['high']), float(history['low']))

    #insert
    try:
        cursor.execute(toInsert)
    except: print("Couldnt push")

    conn.commit()
    conn.close()



if __name__ == "__main__":
    main()
