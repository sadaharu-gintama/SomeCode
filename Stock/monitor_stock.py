################################################################################
# This program is used to get dividend date in the comming week for pre-defined#
# stock list, the information is sent to given Email                           #
# Date: 2016-07-19                                         Programmer: Y. Liu  #
################################################################################
# Version: 0.1
# Update
################################################################################
#!/usr/bin/python

import datetime as dt
# Define Email sending function
def SendEmail(user, pwd, recipient, subject, content):
    import smtplib
    TO = recipient if type(recipient) is list else [recipient]
    message = """From: %s\nTo: %s\nSubject: %s\n\n%s""" % \
              (user, ','.join(TO), subject, content)

    try:
        server = smtplib.SMTP_SSL("smtp.gmail.com", 465)
        server.ehlo()
        server.login(user, pwd)
        server.sendmail(user, TO, message)
        server.close()
        print "Sent sucessfully"
    except Exception as expt:
        print expt


# Get stickers
def GetStickers(filename):
    return [sticker.strip() for sticker in open(filename)]

# Get stock information
def StockInformation(stickers):
    from yahoo_finance import Share
    content = "Stock information today:\n"
    for sticker in stickers:
        share = Share(sticker)
        content += 'Sticker\tBegin Price\tEnd Price\tVolume\t\n'
        content += sticker + '\t' + str(share.get_prev_close()) + '\t' + \
                   str(share.get_price()) + '\t' + \
                   str(share.get_volume()) + '\n'
    share = Share('SPY')
    spy_price = share.get_200day_moving_avg()
    spy_earning = share.get_earnings_share()
    share = Share('TLT')
    tlt_price = share.get_200day_moving_avg()
    tlt_earning = share.get_earnings_share()
    content += 'P/E of SPY: ' + str(float(spy_price) / float(spy_earning)) + '\t'
    content += 'P/E of TLT: ' + str(float(tlt_price) / float(tlt_earning))
    return content

if __name__ == '__main__':
    user = 'yi.liu.197@gmail.com'
    pwd = 'never880325'
    recipient = "yi.liu.197@gmail.com"
    subject = 'Stock Information' + str(dt.datetime.today())
    stickers = GetStickers('stickers.txt')
    content = StockInformation(stickers)
    SendEmail(user, pwd, recipient, subject, content)
