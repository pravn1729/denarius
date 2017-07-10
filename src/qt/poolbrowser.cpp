#include "poolbrowser.h"
#include "ui_poolbrowser.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <QDesktopServices>

#include <sstream>
#include <string>

using namespace json_spirit;

const QString kBaseUrl = "http://denarius.io/dnrusd.php";
const QString kBaseUrl1 = "http://blockchain.info/tobtc?currency=USD&value=1";
const QString kBaseUrl2 = "http://denarius.io/dnrmc.php";
const QString kBaseUrl3 = "http://denarius.io/dnrbtc.php";

QString bitcoinp = "";
QString denariusp = "";
QString dnrmcp = "";
QString dnrbtcp = "";
double bitcoin2;
double denarius2;
double dnrmc2;
double dnrbtc2;
QString bitcoing;
QString dollarg;
int mode=1;
int o = 0;


PoolBrowser::PoolBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PoolBrowser)
{
    ui->setupUi(this);
    setFixedSize(400, 420);


requests();
QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
connect(ui->startButton, SIGNAL(pressed()), this, SLOT( requests()));
connect(ui->egal, SIGNAL(pressed()), this, SLOT( update()));

}

void PoolBrowser::update()
{
    QString temps = ui->egals->text();
    double totald = dollarg.toDouble() * temps.toDouble();
    double totaldq = bitcoing.toDouble() * temps.toDouble();
    ui->egald->setText("$ "+QString::number(totald)+" / "+QString::number(totaldq)+" BTC");

}

void PoolBrowser::requests()
{
	getRequest(kBaseUrl);
    getRequest(kBaseUrl1);
	getRequest(kBaseUrl2);
	getRequest(kBaseUrl3);
}

void PoolBrowser::getRequest( const QString &urlString )
{
    QUrl url ( urlString );
    QNetworkRequest req ( url );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    m_nam.get(req);
}

void PoolBrowser::parseNetworkResponse(QNetworkReply *finished )
{

    QUrl what = finished->url();

    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->error() );
        return;
    }
	
if (what == kBaseUrl) // Denarius Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString denarius = finished->readAll();
    denarius2 = (denarius.toDouble());
    denarius = QString::number(denarius2);
	
    if(denarius > denariusp)
    {
        ui->denarius->setText("<font color=\"green\">$" + denarius + "</font>");
    } else if (denarius < denariusp) {
        ui->denarius->setText("<font color=\"red\">$" + denarius + "</font>");
        } else {
    ui->denarius->setText("$"+denarius+" USD");
    }

    denariusp = denarius;
	dollarg = denarius;
}

if (what == kBaseUrl1) // Bitcoin Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString bitcoin = finished->readAll();
    bitcoin2 = (1 / bitcoin.toDouble());
    bitcoin = QString::number(bitcoin2);
    if(bitcoin > bitcoinp)
    {
        ui->bitcoin->setText("<font color=\"green\">$" + bitcoin + " USD</font>");
    } else if (bitcoin < bitcoinp) {
        ui->bitcoin->setText("<font color=\"red\">$" + bitcoin + " USD</font>");
        } else {
    ui->bitcoin->setText("$"+bitcoin+" USD");
    }

    bitcoinp = bitcoin;
}

if (what == kBaseUrl2) // Denarius Market Cap
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString dnrmc = finished->readAll();
    dnrmc2 = (dnrmc.toDouble());
    dnrmc = QString::number(dnrmc2);
	
    if(dnrmc > dnrmcp)
    {
        ui->dnrmc->setText("<font color=\"green\">$" + dnrmc + "</font>");
    } else if (dnrmc < dnrmcp) {
        ui->dnrmc->setText("<font color=\"red\">$" + dnrmc + "</font>");
        } else {
    ui->dnrmc->setText("$"+dnrmc+" USD");
    }

    dnrmcp = dnrmc;
}

if (what == kBaseUrl3) // Denarius BTC Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString dnrbtc = finished->readAll();
    dnrbtc2 = (dnrbtc.toDouble());
    dnrbtc = QString::number(dnrbtc2);
	
    if(dnrbtc > dnrbtcp)
    {
        ui->dnrbtc->setText("<font color=\"green\">$" + dnrbtc + "</font>");
    } else if (dnrbtc < dnrbtcp) {
        ui->dnrbtc->setText("<font color=\"red\">$" + dnrbtc + "</font>");
        } else {
    ui->dnrbtc->setText("$"+dnrbtc+" USD");
    }

    dnrbtcp = dnrbtc;
	bitcoing = dnrbtc;
}

finished->deleteLater();
}


void PoolBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

PoolBrowser::~PoolBrowser()
{
    delete ui;
}
