function doPost(e) {
  // Pengecekan keamanan dasar
  if (!e || !e.postData || !e.postData.contents) {
    return ContentService.createTextOutput("Error: No POST data received");
  }

  try {
    var sheet = SpreadsheetApp.getActiveSpreadsheet().getActiveSheet();
    
    // Parse JSON langsung dari contents (karena sekarang dikirim sebagai application/json)
    var data = JSON.parse(e.postData.contents);

    var air = Number(data.kualitas_air) || 0;
    var udara = Number(data.kualitas_udara) || 0;
    var tanah = Number(data.kualitas_tanah) || 0;
    var listrik = Number(data.daya_listrik) || 0;

    // Masukkan ke Spreadsheet
    sheet.appendRow([new Date(), air, udara, tanah, listrik]);

    var pesan = [];
    if (air < 0 || air > 100) pesan.push("Kualitas Air tidak wajar: " + air);
    if (udara < 0 || udara > 100) pesan.push("Kualitas Udara tidak wajar: " + udara);
    if (tanah < 0 || tanah > 100) pesan.push("Kualitas Tanah tidak wajar: " + tanah);
    if (listrik < 0 || listrik > 220) pesan.push("Daya Listrik tidak wajar: " + listrik);

    if (pesan.length > 0) {
      MailApp.sendEmail({
        to: "fadhlanakhmad@idn.sch.id",
        subject: "⚠️ Peringatan Sensor",
        body: pesan.join("\n") + "\n\nWaktu: " + new Date()
      });
    }

    return ContentService.createTextOutput("OK - Data Saved");
    
  } catch (error) {
    // Menangkap error jika JSON gagal diparse atau ada masalah lain
    return ContentService.createTextOutput("Error: " + error.toString());
  }
}