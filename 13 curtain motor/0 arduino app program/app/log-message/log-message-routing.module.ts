import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { LogMessagePage } from './log-message.page';

const routes: Routes = [
  {
    path: '',
    component: LogMessagePage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class LogMessagePageRoutingModule {}
