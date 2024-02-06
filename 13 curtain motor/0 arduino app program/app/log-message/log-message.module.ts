import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { LogMessagePageRoutingModule } from './log-message-routing.module';

import { LogMessagePage } from './log-message.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    LogMessagePageRoutingModule
  ],
  declarations: [LogMessagePage],
  exports: [LogMessagePage],
})
export class LogMessagePageModule {}
