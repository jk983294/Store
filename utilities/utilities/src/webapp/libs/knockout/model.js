/**
 * In KnockoutJS, observables are special JavaScript objects that can notify subscribers
 * about changes and automatically detect dependencies.
 * This allows us to synchronize Models and ViewModels when the value of a Model attribute is modified.
 */
var Todo = function ( content, done ) {
    this.content = ko.observable(content);
    this.done = ko.observable(done);
    this.editing = ko.observable(false);
};