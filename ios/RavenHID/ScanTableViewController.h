//
//  ScanTableViewController.h
//  RavenHID
//
//  Created by lucas on 9/21/14.
//  Copyright (c) 2014 Lucas Morris. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLE.h"
#import "DejalActivityView.h"

@protocol  ScanTableViewDelegate <NSObject>
-(void) ScanTableViewControllerDismissed:(NSInteger *)peripheralIndex;
@end


@interface ScanTableViewController : UITableViewController <BLEDelegate> {

}

@property (nonatomic, assign) id<ScanTableViewDelegate> delegate;
@property (strong, nonatomic) IBOutlet UIBarButtonItem *RescanButton;
@property (nonatomic, strong) BLE *bleConnection;
@property (strong, nonatomic) NSMutableArray *mDevices;
@property (strong, nonatomic) NSMutableArray *mDevicesName;

@end
