//
//  ScanTableViewController.m
//  RavenHID
//
//  Created by lucas on 9/21/14.
//  Copyright (c) 2014 Lucas Morris. All rights reserved.
//

#import "ScanTableViewController.h"
#import "ScanTableCell.h"

@interface ScanTableViewController ()

@end

@implementation ScanTableViewController

@synthesize delegate;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _mDevices = [[NSMutableArray alloc] init];
    _mDevicesName = [[NSMutableArray alloc] init];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
    
    if(_bleConnection.peripherals.count > 0) {
        int i;
        for (i = 0; i < _bleConnection.peripherals.count; i++) {
            CBPeripheral *p = [_bleConnection.peripherals objectAtIndex:i];
            
            if (p.identifier.UUIDString != NULL)
            {
                [_mDevices insertObject:p.identifier.UUIDString atIndex:i];
                if (p.name != nil) {
                    [_mDevicesName insertObject:p.name atIndex:i];
                } else {
                    [_mDevicesName insertObject:@"Unknown Device" atIndex:i];
                }
            }
            else
            {
                [_mDevices insertObject:@"NULL" atIndex:i];
                [_mDevicesName insertObject:@"Unknown Device" atIndex:i];
            }
        }
        [self.tableView reloadData];
    } else {
        [_bleConnection findBLEPeripherals:3];
        
        NSLog(@"User clicked on the scan button");
        
        [NSTimer scheduledTimerWithTimeInterval:3.0
                                         target:self
                                       selector:@selector(ScanPostTimer:)
                                       userInfo:nil
                                        repeats:NO];
        
        [DejalBezelActivityView activityViewForView:self.view withLabel:@"Scanning for BLE devices"];
        self.navigationItem.leftBarButtonItem.enabled = NO;
        self.navigationItem.rightBarButtonItem.enabled = NO;
    }
    
}
- (IBAction)ScanClick:(id)sender {
    [_bleConnection findBLEPeripherals:3];
    
    NSLog(@"User clicked on the scan button");
    
    [NSTimer scheduledTimerWithTimeInterval:3.0
                                     target:self
                                   selector:@selector(ScanPostTimer:)
                                   userInfo:nil
                                    repeats:NO];
    
    [DejalBezelActivityView activityViewForView:self.view withLabel:@"Scanning for BLE devices"];
    self.navigationItem.leftBarButtonItem.enabled = NO;
    self.navigationItem.rightBarButtonItem.enabled = NO;
}

- (void)ScanPostTimer:(NSTimer *)timer {
    long selectedIndex = 0;
    
    self.navigationItem.leftBarButtonItem.enabled = YES;
    self.navigationItem.rightBarButtonItem.enabled = YES;
    
    if (_bleConnection.peripherals.count > 0)
    {
        [_mDevices removeAllObjects];
        [_mDevicesName removeAllObjects];
            
        int i;
        for (i = 0; i < _bleConnection.peripherals.count; i++) {
            CBPeripheral *p = [_bleConnection.peripherals objectAtIndex:i];
        
            if (p.identifier.UUIDString != NULL)
            {
                [_mDevices insertObject:p.identifier.UUIDString atIndex:i];
                if (p.name != nil) {
                    [_mDevicesName insertObject:p.name atIndex:i];
                } else {
                    [_mDevicesName insertObject:@"Unknown Device" atIndex:i];
                }
            }
            else
            {
                [_mDevices insertObject:@"NULL" atIndex:i];
                [_mDevicesName insertObject:@"Unknown Device" atIndex:i];
            }
            if(p.state == CBPeripheralStateConnected) {
                selectedIndex = i;
            }
        }
    } else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                  message:@"No BLE Devices found."
                                                  delegate:nil
                                                  cancelButtonTitle:@"OK"
                                                  otherButtonTitles:nil];
        [alert show];
    }
    
    [self.tableView reloadData];
    if(selectedIndex > 0) {
        NSIndexPath *idx = [NSIndexPath indexPathForRow:selectedIndex inSection:1];
        [self.tableView selectRowAtIndexPath:idx animated:YES scrollPosition:UITableViewScrollPositionTop];
    }
    [DejalBezelActivityView removeView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return _mDevices.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"ScanTableCell";
    ScanTableCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    // Configure the cell...
    long row = [indexPath row];
    cell.NameLabel.text = _mDevicesName[row];
    cell.IdentifierLabel.text = _mDevices[row];
    
    return cell;
}

-(NSString *)getUUIDString:(CFUUIDRef)ref {
    NSString *str = [NSString stringWithFormat:@"%@", ref];
    return [[NSString stringWithFormat:@"%@", str] substringWithRange:NSMakeRange(str.length - 36, 36)];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    long row = [indexPath row];
    NSLog(@"Selected row %ld", row);
    
    //Check to see if we think we have an active connection, if so its time to reset.
    if(_bleConnection.activePeripheral) {
        //Check to make sure that connection is in the connected state
        if(_bleConnection.activePeripheral.state == CBPeripheralStateConnected) {
            //If so, then go ahead and cancel it
            [[_bleConnection CM] cancelPeripheralConnection:[_bleConnection activePeripheral]];
        }
    }
    
    if([self.delegate respondsToSelector:@selector(ScanTableViewControllerDismissed:)]) {
        [self.delegate ScanTableViewControllerDismissed:&row];
    }

    [[self navigationController] popViewControllerAnimated:YES];
}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
