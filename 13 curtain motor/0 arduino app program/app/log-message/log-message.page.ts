import { Component, Input, OnInit } from '@angular/core';
import { ModalController } from '@ionic/angular';
import { GlobalService } from '../global.service';
@Component({
  selector: 'app-log-message',
  templateUrl: './log-message.page.html',
  styleUrls: ['./log-message.page.scss'],
})
export class LogMessagePage implements OnInit {

  @Input() messageLog: string[] = [];

  currentPage = 1;
  logsPerPage = 10;
  paginatedLogs: string[] = [];

  constructor(private modalController: ModalController,public globalService: GlobalService,) { }

  ngOnInit() {
    this.messageLog = this.globalService.loadMessageLogFromLocalStorage();
    this.paginateLogs();
  }
  deleteAllLogs() {
    this.messageLog = []; // Clear all log entries
    this.globalService.saveMessageLogToLocalStorage(this.messageLog); // Save the empty array to local storage
    this.paginateLogs(); // Refresh the paginated logs to reflect the deletion
  }
  paginateLogs() {
    const startIndex = (this.currentPage - 1) * this.logsPerPage;
    const endIndex = startIndex + this.logsPerPage;
    this.paginatedLogs = this.messageLog.slice(startIndex, endIndex);
  }
  deleteLog(index: number) {
    // Calculate actual index in messageLog based on pagination
    const actualIndex = (this.currentPage - 1) * this.logsPerPage + index;
    // Remove the log entry
    this.messageLog.splice(actualIndex, 1);
    // Save the updated log to local storage
    this.globalService.saveMessageLogToLocalStorage(this.messageLog);
    // Refresh the paginated logs
    this.paginateLogs();
  }
  changePage(pageNumber: number) {
    this.currentPage = pageNumber;
    this.paginateLogs();
  }

  dismissModal() {
    this.modalController.dismiss();
  }

  get totalPages() {
    return Math.ceil(this.messageLog.length / this.logsPerPage);
  }
}

