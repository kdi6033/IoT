import { ComponentFixture, TestBed, async } from '@angular/core/testing';
import { LogMessagePage } from './log-message.page';

describe('LogMessagePage', () => {
  let component: LogMessagePage;
  let fixture: ComponentFixture<LogMessagePage>;

  beforeEach(async(() => {
    fixture = TestBed.createComponent(LogMessagePage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
